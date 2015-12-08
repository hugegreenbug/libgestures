// Copyright (c) 2012 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gestures/include/fling_to_scroll_filter_interpreter.h"
#include "gestures/include/logging.h"
#include "gestures/include/tracer.h"
#include <math.h>
#include <vector>

namespace gestures {

  FlingToScrollFilterInterpreter::
  FlingToScrollFilterInterpreter(PropRegistry* prop_reg,
				 Interpreter* next,
				 Tracer* tracer)
    : FilterInterpreter(NULL, next, tracer, false),
      scroll_timeout_(0.02f),
      in_fling_(0),
      last_scroll_(0),
      start_time_(0),
      curve_duration_(0.2f),
      fling_to_scroll_enabled_(prop_reg, "Fling To Scroll Enabled", 1) {
    memset(vel_, 0, sizeof(vel_));
    memset(movement_, 0, sizeof(movement_));
    InitName();
  }
  
  void FlingToScrollFilterInterpreter::ProduceGestures(stime_t now) {
    float scroll_x, scroll_y;
    float distance = 1.0;
    
    scroll_x = scroll_y = 0;
    if (now - last_scroll_ < scroll_timeout_)
      return;
   
    if (now - start_time_ >= curve_duration_)
      in_fling_ = false;
    
    if (!in_fling_)
      return;

    distance = 1.0;
    last_scroll_ = now; 
    if (vel_[0] && vel_[0] > 0)
      scroll_x = distance;
    else if (vel_[0] && vel_[0] < 0)
      scroll_x = -distance;

    if (vel_[1] && vel_[1] > 0)
      scroll_y = distance;
    else if (vel_[1] && vel_[1] < 0)
      scroll_y = -distance;

    Log("Fling To Scroll - Produce Gestures - scroll_x: %f scroll_y: %f", scroll_x, scroll_y);
    ProduceGesture(Gesture(kGestureScroll,
			   now,
			   now,
			   scroll_x,
			   scroll_y));
  }
  
  void FlingToScrollFilterInterpreter::
  UpdateTimeouts(stime_t* timeout, stime_t next_timeout, stime_t now) {
    if (in_fling_) {
      if (next_timeout < 0 || next_timeout > scroll_timeout_)
	next_timeout = scroll_timeout_;
    }

    *timeout = next_timeout;
  }


  void FlingToScrollFilterInterpreter::
  SyncInterpretImpl(HardwareState* hwstate, stime_t* timeout) {
    stime_t next_timeout = -1;
    
    if (hwstate->finger_cnt == 2 && in_fling_) {
      in_fling_ = 0;
    }
    Log("FlingToScroll finger count is :%d and in_fling_: %d", hwstate->finger_cnt, in_fling_);
    
    ProduceGestures(hwstate->timestamp);
    next_->SyncInterpret(hwstate, &next_timeout);
    UpdateTimeouts(timeout, next_timeout, hwstate->timestamp);
  }

  void FlingToScrollFilterInterpreter::
  HandleTimerImpl(stime_t now, stime_t* timeout) {
    stime_t next_timeout = -1;
    
    ProduceGestures(now);
    next_->HandleTimer(now, &next_timeout);
    UpdateTimeouts(timeout, next_timeout, now);
  }

  void FlingToScrollFilterInterpreter::
  ConsumeGesture(const Gesture& gesture) {
    if (!fling_to_scroll_enabled_.val_) {
       in_fling_ = 0;
       ProduceGesture(gesture);
       return;
    }

    if (gesture.type == kGestureTypeFling &&
	(gesture.details.fling.vx || gesture.details.fling.vy)) {
      float max_start_velocity = std::max(fabs(gesture.details.fling.vx),
					  fabs(gesture.details.fling.vy));

      movement_[0] = movement_[1] = 0;
      if (max_start_velocity > 0) {
	in_fling_ = 1;
	last_scroll_ = 0;
	curve_duration_ = 0.005f * max_start_velocity;
	vel_[0] = gesture.details.fling.vx;
	vel_[1] = gesture.details.fling.vy;
	start_time_ = gesture.start_time;
	ProduceGestures(gesture.start_time);
      }
    } else if (in_fling_ && gesture.type == kGestureTypeMove) {
      movement_[0] += fabs(gesture.details.move.dx);
      movement_[1] += fabs(gesture.details.move.dy);
      float max_movement = std::max(movement_[0], movement_[1]);
      if (max_movement > 5.0)
       in_fling_ = 0;
    } else if (in_fling_ && gesture.type != kGestureTypeScroll &&
	       gesture.type != kGestureTypeFling &&
	       gesture.type != kGestureTypeButtonsChange) {
      in_fling_ = 0;
    }	

    ProduceGesture(gesture);
  }
}  // namespace gestures
