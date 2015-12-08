// Copyright (c) 2012 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gestures/include/tap_to_click_fix_filter_interpreter.h"
#include "gestures/include/logging.h"
#include "gestures/include/tracer.h"

namespace gestures {

  TapToClickFixFilterInterpreter::
  TapToClickFixFilterInterpreter(PropRegistry* prop_reg,
				 Interpreter* next,
				 Tracer* tracer)
    : FilterInterpreter(NULL, next, tracer, false),
      up_timeout_(0.1f) {
    InitName();
  }

  void TapToClickFixFilterInterpreter::
  Initialize(const HardwareProperties* hwprops,
	     Metrics* metrics,
	     MetricsProperties* mprops,
	     GestureConsumer* consumer) {
    FilterInterpreter::Initialize(hwprops, NULL, mprops, consumer);
    const size_t kMaxButtonDown = 16;
    queue_.DeleteAll();
    free_list_.DeleteAll();
    for (size_t i = 0; i < kMaxButtonDown; ++i) {
      ButtonDown* node = new ButtonDown();
      free_list_.PushBack(node);
    }
  }

  void TapToClickFixFilterInterpreter::
  ProduceGestures(stime_t now) {
    ButtonDown *tap = NULL;
    
    while (!queue_.Empty()) {
      tap = queue_.PopFront();
    
      if (tap && ((now - tap->start_time >= up_timeout_) || !tap->split)) {
	ProduceGesture(Gesture(kGestureButtonsChange,
			       now,
			       now,
			       tap->button_down,
			       tap->button_up));
	free_list_.PushBack(tap);
      } else if (tap) {
	queue_.PushFront(tap);
	break;
      }
    }
  }

  void TapToClickFixFilterInterpreter::
  UpdateTimeouts(stime_t* timeout, stime_t next_timeout, stime_t now) {
    ButtonDown *tap = NULL;
    stime_t next_up_timeout;
    stime_t min_timeout = 0.005;
    
    if (!queue_.Empty()) {
      tap = queue_.PopFront();
      
      next_up_timeout = up_timeout_; 
      if (now - tap->start_time >= up_timeout_) {
        next_up_timeout = min_timeout;
      } else if (now > tap->start_time) {
        next_up_timeout = up_timeout_ - (now - tap->start_time);
	if (next_up_timeout < min_timeout)
	  next_up_timeout = min_timeout;
      }
      
      if (next_timeout < 0 || next_timeout > next_up_timeout)
	next_timeout = next_up_timeout;
      
      queue_.PushFront(tap);
    }

    *timeout = next_timeout;
  }


  void TapToClickFixFilterInterpreter::
  SyncInterpretImpl(HardwareState* hwstate, stime_t* timeout) {
    stime_t next_timeout = -1;

    ProduceGestures(hwstate->timestamp);
    next_->SyncInterpret(hwstate, &next_timeout);
    UpdateTimeouts(timeout, next_timeout, hwstate->timestamp);
  }

  void TapToClickFixFilterInterpreter::
  HandleTimerImpl(stime_t now, stime_t* timeout) {
    stime_t next_timeout = -1;
    
    ProduceGestures(now);
    next_->HandleTimer(now, &next_timeout);
    UpdateTimeouts(timeout, next_timeout, now);
  }

  void TapToClickFixFilterInterpreter::
  ConsumeGesture(const Gesture& gesture) {
    ButtonDown *tap = NULL, *tap2 = NULL;

    if (gesture.type == kGestureTypeButtonsChange &&
	gesture.details.buttons.down == GESTURES_BUTTON_LEFT && 
        gesture.details.buttons.up == GESTURES_BUTTON_LEFT) {
      tap = free_list_.PopBack();
      if (!tap) {
	Err("out of nodes?");
	return;
      }
      tap->start_time = gesture.start_time;
      tap->button_down = gesture.details.buttons.down;
      tap->button_up = GESTURES_BUTTON_NONE;
      tap->split = 1;
      tap2 = free_list_.PopBack();
      if (!tap2) {
	free_list_.PushBack(tap);
	Err("out of nodes?");
	ProduceGestures(gesture.start_time);
	return;
      }
      tap2->start_time = gesture.start_time + up_timeout_;
      tap2->button_up = gesture.details.buttons.down;
      tap2->button_down = GESTURES_BUTTON_NONE;
      tap2->split = 1;
      queue_.PushBack(tap);	    
      queue_.PushBack(tap2);
      ProduceGestures(gesture.start_time);
    } else if (gesture.type == kGestureTypeButtonsChange) {
      tap = free_list_.PopBack();
      if (!tap) {
	Err("out of nodes?");
	return;
      }
      tap->start_time = gesture.start_time;
      tap->button_down = gesture.details.buttons.down;
      tap->button_up = gesture.details.buttons.up;
      tap->split = 0;
      queue_.PushBack(tap);
      ProduceGestures(gesture.start_time);
    } else {
      ProduceGesture(gesture);
    }
  }
}  // namespace gestures
