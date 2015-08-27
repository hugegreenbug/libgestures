// Copyright (c) 2012 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <vector>

#include <gtest/gtest.h>

#include "gestures/include/gestures.h"
#include "gestures/include/tap_to_click_fix_filter_interpreter.h"
#include "gestures/include/string_util.h"
#include "gestures/include/unittest_util.h"
#include "gestures/include/util.h"

namespace gestures {
  class TapToClickFixFilterInterpreterTest : public ::testing::Test {};
  namespace {
    class TapToClickFixFilterInterpreterTestInterpreter : public Interpreter {
    public:
      TapToClickFixFilterInterpreterTestInterpreter()
	: Interpreter(NULL, NULL, false),
	  up_timeout_(0.045f),
	  in_gesture_(0) {}
      
      virtual void SyncInterpret(HardwareState* hwstate, stime_t* timeout) {
	*timeout = up_timeout_;

	if (hwstate->finger_cnt == 1) {
	  ProduceGesture(Gesture(kGestureButtonsChange,
				 hwstate->timestamp,
				 hwstate->timestamp,
				 GESTURES_BUTTON_LEFT,
				 GESTURES_BUTTON_NONE));
	  in_gesture_ = 1;
	  *timeout = up_timeout_;
	} else {
	  in_gesture_ = 0;
	  *timeout = -1;
	}	  
      }
      
      virtual void HandleTimer(stime_t now, stime_t* timeout) {
	*timeout = -1;
	if (in_gesture_) {
	  ProduceGesture(Gesture(kGestureButtonsChange,
				 now,
				 now,
				 GESTURES_BUTTON_NONE,
				 GESTURES_BUTTON_LEFT));	 
	}
      }
      
      stime_t up_timeout_;
      int in_gesture_;
    };
  }
  // Does two tap gestures, one with keyboard interference.
  TEST(TapToClickFixFilterInterpreterTest, TapToClickFixTest) {
    std::unique_ptr<TapToClickFixFilterInterpreter> ti;

    HardwareProperties hwprops = {
      0,  // left edge
      0,  // top edge
      200,  // right edge
      200,  // bottom edge
      1.0,  // pixels/TP width
      1.0,  // pixels/TP height
      1.0,  // screen DPI x
      1.0,  // screen DPI y
      -1,  // orientation minimum
      2,   // orientation maximum
      5,  // max fingers
      5,  // max touch
      0,  // t5r2
      0,  // semi-mt
      1,  // is button pad
      0   // has_wheel
    };
    TestInterpreterWrapper wrapper(ti.get(), &hwprops);

    FingerState fs = {
      // TM, Tm, WM, Wm, Press, Orientation, X, Y, TrID
      0, 0, 0, 0, 50, 0, 4, 4, 91, 0
    };
    HardwareState hwstates[] = {
      // Simple 1-finger tap
      { 0.01, 0, 1, 1, &fs, 0, 0, 0, 0 },
      { 0.02, 0, 0, 0, NULL, 0, 0, 0, 0 },
    };

    enum {
      kTapToClick = 0,
      kMaxTests
    };
    for (size_t test = 0; test != kMaxTests; test++) {
      TapToClickFixFilterInterpreterTestInterpreter* base_interpreter =
	new TapToClickFixFilterInterpreterTestInterpreter;
      ti.reset(new TapToClickFixFilterInterpreter(NULL, base_interpreter, NULL));
      wrapper.Reset(ti.get());
      stime_t timeout = -1;
      Gesture *gs = wrapper.SyncInterpret(&hwstates[test], &timeout);
      EXPECT_EQ(GESTURES_BUTTON_LEFT, gs->details.buttons.down);
      EXPECT_EQ(GESTURES_BUTTON_NONE, gs->details.buttons.up);
      gs = wrapper.HandleTimer(hwstates[test].timestamp, &timeout);
      EXPECT_EQ(GESTURES_BUTTON_NONE, gs->details.buttons.down);
      EXPECT_EQ(GESTURES_BUTTON_LEFT, gs->details.buttons.up);
    }
  }
}
