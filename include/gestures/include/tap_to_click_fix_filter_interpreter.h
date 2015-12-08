// Copyright (c) 2012 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>  // for FRIEND_TEST

#include "gestures/include/filter_interpreter.h"
#include "gestures/include/immediate_interpreter.h"
#include "gestures/include/gestures.h"
#include "gestures/include/list.h"
#include "gestures/include/prop_registry.h"
#include "gestures/include/tracer.h"

#ifndef GESTURES_TAP_TO_CLICK_FIX_FILTER_INTERPRETER_H_
#define GESTURES_TAP_TO_CLICK_FIX_FILTER_INTERPRETER_H_

namespace gestures {

  class TapToClickFixFilterInterpreter : public FilterInterpreter {
  public:
    // Takes ownership of |next|:
    TapToClickFixFilterInterpreter(PropRegistry* prop_reg,
				   Interpreter* next,
				   Tracer* tracer);
    virtual ~TapToClickFixFilterInterpreter() {}

  protected:
    virtual void SyncInterpretImpl(HardwareState* hwstate, stime_t* timeout);
    virtual void HandleTimerImpl(stime_t now, stime_t* timeout);
    virtual void ConsumeGesture(const Gesture& gesture);
    virtual void Initialize(const HardwareProperties* hwprops,
			    Metrics* metrics,
			    MetricsProperties* mprops,
			    GestureConsumer* consumer);
  private:
    float up_timeout_;
    
    struct ButtonDown {
      int button_down;
      int button_up;
      int split;
      stime_t start_time;
 
      ButtonDown* next_;
      ButtonDown* prev_;
    };
    
    List<ButtonDown> queue_;
    List<ButtonDown> free_list_;
    void UpdateTimeouts(stime_t* timeout, stime_t next_timeout, stime_t now);
    void ProduceGestures(stime_t now);
  };

}  // namespace gestures

#endif  // GESTURES_SCALING_FILTER_INTERPRETER_H_
