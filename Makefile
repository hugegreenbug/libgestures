# Copyright (c) 2012 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

OBJDIR = obj
SRC=$(shell readlink -f .)

# Objects for libgestures
SO_OBJECTS=\
	$(OBJDIR)/accel_filter_interpreter.o \
	$(OBJDIR)/activity_log.o \
	$(OBJDIR)/box_filter_interpreter.o \
	$(OBJDIR)/click_wiggle_filter_interpreter.o \
	$(OBJDIR)/filter_interpreter.o \
	$(OBJDIR)/finger_merge_filter_interpreter.o \
	$(OBJDIR)/finger_metrics.o \
	$(OBJDIR)/fling_stop_filter_interpreter.o \
	$(OBJDIR)/gestures.o \
	$(OBJDIR)/iir_filter_interpreter.o \
	$(OBJDIR)/immediate_interpreter.o \
	$(OBJDIR)/integral_gesture_filter_interpreter.o \
	$(OBJDIR)/interpreter.o \
	$(OBJDIR)/logging_filter_interpreter.o \
	$(OBJDIR)/lookahead_filter_interpreter.o \
	$(OBJDIR)/metrics_filter_interpreter.o \
	$(OBJDIR)/mouse_interpreter.o \
	$(OBJDIR)/multitouch_mouse_interpreter.o \
	$(OBJDIR)/non_linearity_filter_interpreter.o \
	$(OBJDIR)/palm_classifying_filter_interpreter.o \
	$(OBJDIR)/prop_registry.o \
	$(OBJDIR)/scaling_filter_interpreter.o \
	$(OBJDIR)/cr48_profile_sensor_filter_interpreter.o \
	$(OBJDIR)/sensor_jump_filter_interpreter.o \
	$(OBJDIR)/split_correcting_filter_interpreter.o \
	$(OBJDIR)/stationary_wiggle_filter_interpreter.o \
	$(OBJDIR)/stuck_button_inhibitor_filter_interpreter.o \
	$(OBJDIR)/t5r2_correcting_filter_interpreter.o \
	$(OBJDIR)/trace_marker.o \
	$(OBJDIR)/tracer.o \
	$(OBJDIR)/trend_classifying_filter_interpreter.o \
	$(OBJDIR)/util.o \
	$(OBJDIR)/values.o \
	$(OBJDIR)/json_reader.o \
	$(OBJDIR)/json_writer.o \
	$(OBJDIR)/json_parser.o \
	$(OBJDIR)/string_util.o \
	$(OBJDIR)/stringprintf.o \
	$(OBJDIR)/string16.o \
	$(OBJDIR)/logging.o \
	$(OBJDIR)/icu_utf.o \
	$(OBJDIR)/file_util.o \
	$(OBJDIR)/file_util_linux.o \
	$(OBJDIR)/file_util_posix.o \
	$(OBJDIR)/file_path.o \
	$(OBJDIR)/utf_string_conversions.o \
	$(OBJDIR)/atomicops_internals_x86_gcc.o \
	$(OBJDIR)/at_exit.o \
	$(OBJDIR)/thread_restrictions.o \
	$(OBJDIR)/singleton.o \
	$(OBJDIR)/platform_thread_posix.o \
	$(OBJDIR)/lock.o \
	$(OBJDIR)/debugger.o \
	$(OBJDIR)/debugger_posix.o \
	$(OBJDIR)/command_line.o \
	$(OBJDIR)/base_switches.o \
	$(OBJDIR)/base_paths.o \
	$(OBJDIR)/base_paths_posix.o \
	$(OBJDIR)/gtest-typed-test.o \
	$(OBJDIR)/gtest-test-part.o \
	$(OBJDIR)/gtest-printers.o \
	$(OBJDIR)/gtest-port.o \
	$(OBJDIR)/gtest-filepath.o \
	$(OBJDIR)/gtest-death-test.o \
	$(OBJDIR)/gtest.o \
	$(OBJDIR)/string_escape.o \
	$(OBJDIR)/string_number_conversions.o \
	$(OBJDIR)/string_piece.o \
	$(OBJDIR)/lock_impl_posix.o \
	$(OBJDIR)/stack_trace.o \
	$(OBJDIR)/stack_trace_posix.o \
	$(OBJDIR)/alias.o \
	$(OBJDIR)/safe_strerror_posix.o \
	$(OBJDIR)/vlog.o \
	$(OBJDIR)/platform_file.o \
	$(OBJDIR)/platform_file_posix.o \
	$(OBJDIR)/sys_string_conversions_posix.o \
	$(OBJDIR)/pickle.o \
	$(OBJDIR)/utf_string_conversion_utils.o \
	$(OBJDIR)/utf_offset_string_conversions.o \
	$(OBJDIR)/ref_counted.o \
	$(OBJDIR)/ref_counted_memory.o \
	$(OBJDIR)/callback_internal.o \
	$(OBJDIR)/lazy_instance.o \
	$(OBJDIR)/thread_local_posix.o \
	$(OBJDIR)/time.o \
	$(OBJDIR)/time_posix.o \
	$(OBJDIR)/tracked_objects.o \
	$(OBJDIR)/string_split.o \
	$(OBJDIR)/path_service.o \
	$(OBJDIR)/process_util_linux.o \
	$(OBJDIR)/environment.o \
	$(OBJDIR)/xdg_util.o \
	$(OBJDIR)/g_fmt.o \
	$(OBJDIR)/thread_collision_warner.o \
	$(OBJDIR)/prtime.o \
	$(OBJDIR)/location.o \
	$(OBJDIR)/tracked_time.o \
	$(OBJDIR)/process_util_posix.o \
	$(OBJDIR)/thread_local_storage_posix.o \
	$(OBJDIR)/dtoa.o \
	$(OBJDIR)/alternate_timer.o \
	$(OBJDIR)/process_util.o \
	$(OBJDIR)/xdg_user_dir_lookup.o \
	$(OBJDIR)/dynamic_annotations.o \
	$(OBJDIR)/type_profiler_control.o \
	$(OBJDIR)/type_profiler.o \
	$(OBJDIR)/file_descriptor_shuffle.o \
	$(OBJDIR)/sys_info.o \
	$(OBJDIR)/sys_info_linux.o \
	$(OBJDIR)/sys_info_posix.o \
	$(OBJDIR)/process_linux.o \
	$(OBJDIR)/process_posix.o 

# Objects for unittests
TEST_OBJECTS=\
	$(OBJDIR)/accel_filter_interpreter_unittest.o \
	$(OBJDIR)/activity_log_unittest.o \
	$(OBJDIR)/activity_replay_unittest.o \
	$(OBJDIR)/box_filter_interpreter_unittest.o \
	$(OBJDIR)/click_wiggle_filter_interpreter_unittest.o \
	$(OBJDIR)/fling_stop_filter_interpreter_unittest.o \
	$(OBJDIR)/gestures_unittest.o \
	$(OBJDIR)/iir_filter_interpreter_unittest.o \
	$(OBJDIR)/immediate_interpreter_unittest.o \
	$(OBJDIR)/integral_gesture_filter_interpreter_unittest.o \
	$(OBJDIR)/interpreter_unittest.o \
	$(OBJDIR)/list_unittest.o \
	$(OBJDIR)/logging_filter_interpreter_unittest.o \
	$(OBJDIR)/lookahead_filter_interpreter_unittest.o \
	$(OBJDIR)/non_linearity_filter_interpreter_unittest.o \
	$(OBJDIR)/map_unittest.o \
	$(OBJDIR)/mouse_interpreter_unittest.o \
	$(OBJDIR)/multitouch_mouse_interpreter_unittest.o \
	$(OBJDIR)/palm_classifying_filter_interpreter_unittest.o \
	$(OBJDIR)/prop_registry_unittest.o \
	$(OBJDIR)/scaling_filter_interpreter_unittest.o \
	$(OBJDIR)/cr48_profile_sensor_filter_interpreter_unittest.o \
	$(OBJDIR)/sensor_jump_filter_interpreter_unittest.o \
	$(OBJDIR)/set_unittest.o \
	$(OBJDIR)/split_correcting_filter_interpreter_unittest.o \
	$(OBJDIR)/stuck_button_inhibitor_filter_interpreter_unittest.o \
	$(OBJDIR)/t5r2_correcting_filter_interpreter_unittest.o \
	$(OBJDIR)/trace_marker_unittest.o \
	$(OBJDIR)/tracer_unittest.o \
	$(OBJDIR)/unittest_util.o \
	$(OBJDIR)/util_unittest.o \
	$(OBJDIR)/vector_unittest.o

# Objects that are neither unittests nor SO objects
#MISC_OBJECTS=\
#	$(OBJDIR)/activity_replay.o \

MISC_OBJECTS=
TEST_MAIN=\
	$(OBJDIR)/test_main.o

TEST_EXE=test
SONAME=$(OBJDIR)/libgestures.so.0

#ALL_OBJECTS=\
#	$(TEST_OBJECTS) \
#	$(TEST_MAIN) \
#	$(SO_OBJECTS) \
#	$(MISC_OBJECTS)

ALL_OBJECTS=\
	$(TEST_MAIN) \
	$(SO_OBJECTS)

#ALL_OBJECT_FILES=\
#	$(SO_OBJECTS) \
#	$(MISC_OBJECTS) \
#	$(TEST_OBJECTS) \
#	$(TEST_MAIN)

ALL_OBJECT_FILES=\
	$(SO_OBJECTS) \
	$(TEST_MAIN)

DEPDIR = .deps

DESTDIR = 

CXXFLAGS+=\
	-g \
	-std=gnu++11 \
	-fno-exceptions \
	-fno-strict-aliasing \
	-fPIC \
	-Wclobbered \
	-Wempty-body \
	-Wignored-qualifiers \
	-Wsign-compare \
	-Wtype-limits \
	-D__STDC_FORMAT_MACROS=1 \
	-D_FILE_OFFSET_BITS=64 \
	-DGESTURES_INTERNAL=1 \
	-Iinclude \
	`pkg-config --cflags glib-2.0`

LID_TOUCHPAD_HELPER=lid_touchpad_helper

# Local compilation needs these flags, esp for code coverage testing
ifeq (g++,$(CXX))
CXXFLAGS+=\
	-O1 \
	-DVCSID="\"0.1 LINUX\"" \
	--coverage \
	-ftest-coverage \
	-fprofile-arcs
LINK_FLAGS+=-lgcov `pkg-config --libs glib-2.0`
else
CXXFLAGS+=\
	-DXLOGGING
endif

#PKG_CONFIG ?= pkg-config
#BASE_VER ?= 180609
#PC_DEPS += libchrome-$(BASE_VER)
#PC_CFLAGS := $(shell $(PKG_CONFIG) --cflags $(PC_DEPS))
#PC_LIBS := $(shell $(PKG_CONFIG) --libs $(PC_DEPS))

#CXXFLAGS += $(PC_CFLAGS)
#LINK_FLAGS += $(PC_LIBS)

LINK_FLAGS+=\
	-lpthread \
	-lrt

#TEST_LINK_FLAGS=\
#	-lgcov \
#	-lglib-2.0 \
#	-lgtest

TEST_LINK_FLAGS=

all: $(SONAME)
	$(MAKE) -C $(LID_TOUCHPAD_HELPER)

$(SONAME): $(SO_OBJECTS)
	$(CXX) -shared -o $@ $(SO_OBJECTS) -Wl,-h$(SONAME:$(OBJDIR)/%=%) \
		$(LINK_FLAGS)

$(TEST_EXE): $(ALL_OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $(ALL_OBJECTS) $(LINK_FLAGS) $(TEST_LINK_FLAGS)

$(OBJDIR)/%.o : src/%.cc
	mkdir -p $(OBJDIR) $(DEPDIR) || true
	$(CXX) $(CXXFLAGS) -MD -c -o $@ $<
	@mv $(@:$.o=$.d) $(DEPDIR)

$(OBJDIR)/%.o : src/%.c
	mkdir -p $(OBJDIR) $(DEPDIR) || true
	$(CC) $(CXXFLAGS) -MD -c -o $@ $<
	@mv $(@:$.o=$.d) $(DEPDIR)

LIBDIR = /usr/lib

install: $(SONAME)
#	$(MAKE) -C $(LID_TOUCHPAD_HELPER) install
	install -D -m 0755 $(SONAME) \
		$(DESTDIR)$(LIBDIR)/$(SONAME:$(OBJDIR)/%=%)
	ln -sf $(SONAME:$(OBJDIR)/%=%) \
		$(DESTDIR)$(LIBDIR)/$(SONAME:$(OBJDIR)/%.0=%)
	install -D -m 0644 \
		include/gestures.h $(DESTDIR)/usr/include/gestures/gestures.h

clean:
	$(MAKE) -C $(LID_TOUCHPAD_HELPER) clean
	rm -rf $(OBJDIR) $(DEPDIR) $(TEST_EXE) html app.info app.info.orig

setup-in-place:
	mkdir -p $(SRC)/in-place/gestures || true
	ln -sf $(SRC)/include/gestures.h $(SRC)/in-place/gestures/gestures.h
	ln -sf $(SRC)/$(SONAME) $(SRC)/in-place/$(SONAME:$(OBJDIR)/%.0=%)
	ln -sf $(SRC)/$(SONAME) $(SRC)/in-place/$(SONAME:$(OBJDIR)/%=%)

in-place: CXXFLAGS+=-DGESTURES_LARGE_LOGGING_BUFFER
in-place: $(SONAME)

clean-in-place: clean

# Unittest coverage

LCOV_EXE=/usr/bin/lcov

$(LCOV_EXE):
	sudo emerge -DNuv1 dev-util/lcov

cov: $(TEST_EXE) $(LCOV_EXE)
	lcov -d . --zerocounters
	./$(TEST_EXE)
	lcov --directory . --capture --output-file $(OBJDIR)/app.info
	sed -i.orig 's|/obj/src/|/src/|g' $(OBJDIR)/app.info
	sed -i.orig 's|/gestures/gestures/|/gestures/|g' $(OBJDIR)/app.info
	genhtml --no-function-coverage -o html $(OBJDIR)/app.info || \
		genhtml -o html $(OBJDIR)/app.info
	./tools/local_coverage_rate.sh $(OBJDIR)/app.info

.PHONY : clean cov all

-include $(ALL_OBJECT_FILES:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
