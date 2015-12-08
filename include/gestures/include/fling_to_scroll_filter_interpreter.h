// Copyright (c) 2012 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>  // for FRIEND_TEST

#include "gestures/include/filter_interpreter.h"
#include "gestures/include/gestures.h"
#include "gestures/include/list.h"
#include "gestures/include/prop_registry.h"
#include "gestures/include/tracer.h"

#ifndef GESTURES_FLING_TO_SCROLL_FILTER_INTERPRETER_H_
#define GESTURES_FLING_TO_SCROLL_FILTER_INTERPRETER_H_

namespace gestures {

  class FlingToScrollFilterInterpreter : public FilterInterpreter {
  public:
    // Takes ownership of |next|:
    FlingToScrollFilterInterpreter(PropRegistry* prop_reg,
				   Interpreter* next,
				   Tracer* tracer);
    virtual ~FlingToScrollFilterInterpreter() {}

  protected:
    virtual void SyncInterpretImpl(HardwareState* hwstate, stime_t* timeout);
    virtual void HandleTimerImpl(stime_t now, stime_t* timeout);
    virtual void ConsumeGesture(const Gesture& gesture);
  private:
    float scroll_timeout_;
    int in_fling_;
    stime_t last_scroll_;
    stime_t start_time_;
    double curve_duration_;
    double vel_[2];
    float movement_[2];
    BoolProperty fling_to_scroll_enabled_;
    void UpdateTimeouts(stime_t* timeout, stime_t next_timeout, stime_t now);
    void ProduceGestures(stime_t now);
  };
}  // namespace gestures

#endif  // FLING_TO_SCROLL_FILTER_INTERPRETER_H_
