# Simple regression tester:
#
# Make sure 'asasm' can be found in your PATH when executing this Makefile
# or that you specify it with the variable AS via the 'make' command line.
#
# Tests which are supposed to pass are located in the 'tests_pass' directory.
# Tests which are known to fail (but are supposed to work) are located in
# the 'tests_fail' directory.
#
# When a test which is known to pass but no longer is, its actual (wrong)
# output will be renamed as output_actual/<test>_failed.o and its reference
# is output_reference/<test>.o.  The source assembler for that test is
# tests_pass/<test>.s.
#
# For testing all test cases which are known to pass:
#   $ make
# Or
#   $ make AS=my_asasm
#
# For testing one particular test (including a failing one):
#   $ make output_actual/<specific_test>.o
# Or
#   $ make AS=my_asm output_actual/<specific_test>.o
#
# To clean up after testing or to re-do all tests again:
#   $ make clean
#
# To specify extra ASASM command line parameters (like "-APCS"), define them
# in one of the first 5 lines of the assembler input file using keyword RUNOPT:
# E.g.:
#   ; RUNOPT: -APCS 3
# RUNOPT is used for both generating the actual and reference output.  RUNOPTACT
# and RUNOPTREF is used for respectively generate teh actual and refernece output.

# Environment variables needed in one of the tests:
export HDR_PATH := $(CURDIR)/tests_pass/include/hdr
export APCS := APCS-32

# EXTENSION enables extra tests for functionality ASASM has but ObjAsm hasn't.

AS := asasm
ASFLAGS := -aof -PD "EXTENSION SETL {TRUE}"
DIFF := diff
DIFFFLAGS := -q
ECHO := echo
ECHOFLAGS :=
MKDIR := mkdir

V := @

check_PROGRAMS_PASS := \
	$(wildcard option_apcs/*.s) \
	$(wildcard option_cpu/*.s) \
	$(wildcard option_device/*.s) \
	$(wildcard option_fpu/*.s) \
	$(wildcard tests_pass/*.s)
check_PROGRAMS_FAIL := \
	$(wildcard tests_fail/*.s)
ref_PROGRAMS = \
	$(check_PROGRAMS_PASS:option_apcs/%.s=output_reference/%.o) \
	$(check_PROGRAMS_PASS:option_cpu/%.s=output_reference/%.o) \
	$(check_PROGRAMS_PASS:option_device/%.s=output_reference/%.o) \
	$(check_PROGRAMS_PASS:option_fpu/%.s=output_reference/%.o) \
	$(check_PROGRAMS_FAIL:tests_fail/%.s=output_reference/%.o) \
	$(check_PROGRAMS_PASS:tests_pass/%.s=output_reference/%.o)
act_PROGRAMS = \
	$(check_PROGRAMS_PASS:option_apcs/%.s=output_actual/%.o) \
	$(check_PROGRAMS_PASS:option_cpu/%.s=output_actual/%.o) \
	$(check_PROGRAMS_PASS:option_device/%.s=output_actual/%.o) \
	$(check_PROGRAMS_PASS:option_fpu/%.s=output_actual/%.o) \
	$(check_PROGRAMS_PASS:tests_pass/%.s=output_actual/%.o)

VPATH = option_apcs:option_cpu:option_device:option_fpu:tests_pass:tests_fail

.PHONY: all clean

all: $(act_PROGRAMS)
	@$(ECHO) "*** All tests were OK (do 'make clean' if you want to re-do them)"

clean:
	-rm -fr output_actual
	-rm -fr output_reference

output_actual/%.o: %.s output_reference/%.o Makefile
	@$(ECHO) +++ $< : Assemble actual and compare with reference
	$(V)$(MKDIR) -p output_actual
	$(V)$(AS) $(ASFLAGS) $(shell head -5 $< | grep -E "RUNOPT(ACT|):" | sed -r -e "s/^.*RUNOPT(ACT|)://") -PD "REFERENCE SETL {FALSE}" -o $@ $<
	$(V)$(DIFF) $(DIFFFLAGS) $@ $(@:output_actual/%=output_reference/%) || (mv $@ $(@:%.o=%_failed.o) && false)

output_reference/%.o: %.s Makefile
	@$(ECHO) +++ $< : Create reference $@
	$(V)$(MKDIR) -p output_reference
	$(V)$(AS) $(ASFLAGS) $(shell head -5 $< | grep -E "RUNOPT(REF|):" | sed -r -e "s/^.*RUNOPT(REF|)://") -PD "REFERENCE SETL {TRUE}" -o $@ $<
