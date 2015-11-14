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
      scroll_timeout_(0.05f),
      last_scroll_(0.0),
      in_fling_(0),
      curve_duration_(0.2f),
      last_velocity_(0),
      start_timestamp_(0),
      previous_timestamp_(0),
      time_offset_(0),
      position_offset_(0),
      fling_to_scroll_enabled_(prop_reg, "Fling To Scroll Enabled", 1) {
    InitName();
    memset(cumulative_scroll_, 0, sizeof(double) * 2);
    memset(displacement_ratio_, 0, sizeof(double) * 2);
    memset(movement_, 0, sizeof(float) * 2);
  }
  
  double FlingToScrollFilterInterpreter::GetPositionAtTime(double t) {
    return kDefaultAlpha * exp(-kDefaultGamma * t) - kDefaultBeta * t -
      kDefaultAlpha;
  }
  
  double FlingToScrollFilterInterpreter::GetVelocityAtTime(double t) {
    return -kDefaultAlpha * kDefaultGamma * exp(-kDefaultGamma * t) -
      kDefaultBeta;
  }
  
  double FlingToScrollFilterInterpreter::GetTimeAtVelocity(double v) {
    return (-log((v + kDefaultBeta) / (-kDefaultAlpha * kDefaultGamma)) /
	    kDefaultGamma);
  }
  
  void FlingToScrollFilterInterpreter::ProduceGestures(stime_t now) {
    double delta[2];
    
    if (now - last_scroll_ < scroll_timeout_)
      return;
   
    last_scroll_ = now; 
    memset(delta, 0, sizeof(double) * 2);
    if (!in_fling_) {
      return;
    }

    
    in_fling_ = ComputeScrollDeltaAtTime(now, delta);
    if (!in_fling_)
	return;

    Log("Fling To Scroll - Produce Gestures - scroll_x: %f scroll_y: %f", delta[0], delta[1]);
    ProduceGesture(Gesture(kGestureScroll,
			   now,
			   now,
			   delta[0],
			   delta[1]));
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

  bool FlingToScrollFilterInterpreter::
    ComputeScrollOffset(stime_t time,
			double* offset,
			double* velocity) {
    stime_t elapsed_time = (time - start_timestamp_);
    bool still_active = true;
    float scalar_offset;
    float scalar_velocity, temp_velocity;

    double offset_time = elapsed_time + time_offset_;
    if (offset_time < curve_duration_) {
      scalar_offset = GetPositionAtTime(offset_time / 1000) - position_offset_;
      scalar_velocity = GetVelocityAtTime(offset_time);
    } else {
      scalar_offset = GetPositionAtTime(curve_duration_) - position_offset_;
      scalar_velocity = 0;
      still_active = false;
    }

    temp_velocity = scalar_velocity;
    scalar_velocity -= last_velocity_;
    last_velocity_ = temp_velocity;
    Log("FlingToScroll compute - scaler_offset: %f , velocity: %f",
	scalar_offset, scalar_velocity);
    *offset = scalar_offset;
    *velocity = scalar_velocity;
    
    return still_active;
  }
  
  bool FlingToScrollFilterInterpreter::ComputeScrollDeltaAtTime(stime_t current,
								double* delta) {
    if (current <= previous_timestamp_) {
      return true;
    }
    
    previous_timestamp_ = current;
    
    double offset, velocity;
    bool still_active = ComputeScrollOffset(current, &offset, &velocity);

    if (!offset)
      return still_active;

    delta[0] = displacement_ratio_[0] * offset;
    delta[1] = displacement_ratio_[1] * offset;
    
    cumulative_scroll_[0] = delta[0];
    cumulative_scroll_[1] = delta[1];
    
    return still_active;
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
      in_fling_ = 1;
      last_scroll_ = 0;
      float max_start_velocity = std::max(fabs(gesture.details.fling.vx),
					  fabs(gesture.details.fling.vy));
      if (max_start_velocity > GetVelocityAtTime(0))
	max_start_velocity = GetVelocityAtTime(0);

      movement_[0] = movement_[1] = 0;
      if (max_start_velocity > 0) {
	curve_duration_ = std::min((0.0012f * max_start_velocity), 0.85f);
        last_velocity_ = 0;
	memset(cumulative_scroll_, 0, sizeof(double) * 2);
	start_timestamp_ = gesture.start_time;
	previous_timestamp_ = gesture.start_time;
	displacement_ratio_[0] = gesture.details.fling.vx/max_start_velocity;
	displacement_ratio_[1] = gesture.details.fling.vy/max_start_velocity;
	time_offset_ = 0;
	position_offset_ = GetPositionAtTime(time_offset_);

	ProduceGestures(gesture.start_time);
      }
    } else if (in_fling_ && gesture.type == kGestureTypeMove) {
      movement_[0] += fabs(gesture.details.move.dx);
      movement_[1] += fabs(gesture.details.move.dy);
      float max_movement = std::max(movement_[0], movement_[1]);
      if (max_movement > 5.0)
       in_fling_ = 0;
    } else if (in_fling_ && gesture.type != kGestureTypeScroll &&
	       gesture.type != kGestureTypeFling) {
      in_fling_ = 0;
    }	

    if (!in_fling_ || (gesture.type != kGestureTypeButtonsChange && gesture.type != kGestureTypeScroll))
      ProduceGesture(gesture);
  }
}  // namespace gestures
