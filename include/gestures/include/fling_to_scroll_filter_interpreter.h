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
    const float kDefaultAlpha = -5.70762e+03f;
    const float kDefaultBeta = 1.72e+02f;
    const float kDefaultGamma = 3.7e+00f;
    double curve_duration_;
    double mid_curve_duration_;
    stime_t start_timestamp_;
    stime_t previous_timestamp_;
    double time_offset_;
    double position_offset_;
    double two_fingers_detected_time_;
    double stop_delay_;
    double displacement_ratio_[2];
    double cumulative_scroll_[2];
    float movement_[2];
    BoolProperty fling_to_scroll_enabled_;
    void UpdateTimeouts(stime_t* timeout, stime_t next_timeout, stime_t now);
    void ProduceGestures(stime_t now);
    bool ComputeScrollOffset(stime_t time, double* offset, double* velocity);
    bool ComputeScrollDeltaAtTime(stime_t current, double* delta);
    double GetPositionAtTime(double t);
    double GetVelocityAtTime(double t);
    double GetTimeAtVelocity(double v);
  };
}  // namespace gestures

#endif  // FLING_TO_SCROLL_FILTER_INTERPRETER_H_
