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

# Environment variables needed in the tests:
export HDR_PATH := $(CURDIR)/hdr
export APCS := APCS-32

# EXTENSION is a test for enable testing extra functionality ASASM has
# compared to objasm.

AS = asasm
ASFLAGS = -t xscale -aof -PD "EXTENSION SETL {TRUE}"
DIFF = diff
DIFFFLAGS = -q
ECHO = echo
ECHOFLAGS =
MKDIR = mkdir

check_PROGRAMS_PASS := $(wildcard tests_pass/*.s)
check_PROGRAMS_FAIL := $(wildcard tests_fail/*.s)
ref_PROGRAMS = $(check_PROGRAMS_PASS:tests_pass/%.s=output_reference/%.o) \
	$(check_PROGRAMS_FAIL:tests_fail/%.s=output_reference/%.o)
act_PROGRAMS = $(check_PROGRAMS_PASS:tests_pass/%.s=output_actual/%.o)

VPATH = tests_pass:tests_fail

.PHONY: all clean

all: $(act_PROGRAMS)
	@$(ECHO) "*** All tests were OK (do 'make clean' if you want to re-do them)"

clean:
	-rm -fr output_actual
	-rm -fr output_reference

output_actual/%.o: %.s output_reference/%.o Makefile
	@$(ECHO) +++ $< : Assemble actual and compare with reference
	@$(MKDIR) -p output_actual
	@$(AS) $(ASFLAGS) -PD "REFERENCE SETL {FALSE}" -o $@ $<
	@$(DIFF) $(DIFFFLAGS) $@ $(@:output_actual/%=output_reference/%) || (mv $@ $(@:%.o=%_failed.o) && false)

output_reference/%.o: %.s Makefile
	@$(ECHO) +++ $< : Create reference $@
	@$(MKDIR) -p output_reference
	@$(AS) $(ASFLAGS) -PD "REFERENCE SETL {TRUE}" -o $@ $<
