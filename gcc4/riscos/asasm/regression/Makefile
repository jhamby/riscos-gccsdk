# Simple regression tester:
#
# Make sure 'asasm' can be found in your PATH when executing this Makefile.
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
#
# For testing one particular test (including a failing one):
#   $ make output_actual/<specific_test>.o
#
# To clean up after testing:
#   $ make clean

export HDR_PATH := $(CURDIR)/hdr
export APCS := APCS-32

AS = asasm
ASFLAGS = -t xscale -objasm -aof
DIFF = diff
DIFFFLAGS = -q
ECHO = echo
ECHOFLAGS =
MKDIR = mkdir

check_PROGRAMS_PASS := $(wildcard tests_pass/*.s)
check_PROGRAMS_FAIL := $(wildcard tests_fail/*.s)
ref_PROGRAMS = $(check_PROGRAMS_PASS:tests_pass/%.s=output_reference/%.o) $(check_PROGRAMS_FAIL:tests_fail/%.s=output_reference/%.o)
act_PROGRAMS = $(check_PROGRAMS_PASS:tests_pass/%.s=output_actual/%.o)

VPATH = tests_pass:tests_fail

all: $(act_PROGRAMS)
	@$(ECHO) All tests were OK

clean:
	-rm -fr output_actual
	-rm -fr output_reference
	-rm -f config.h

output_actual/%.o: %.s output_reference/%.o Makefile
	@$(ECHO) +++ $< : Assemble actual and compare with reference
	@$(MKDIR) -p output_actual
	@$(ECHO) $(ECHOFLAGS) "\tGBLL REFERENCE\nREFERENCE\tSETL\t{FALSE}\n\tEND" >config.h
	@$(AS) $(ASFLAGS) -o $@ $<
	@$(DIFF) $(DIFFFLAGS) $@ $(@:output_actual/%=output_reference/%) || (mv $@ $(@:%.o=%_failed.o) && false)

output_reference/%.o: %.s Makefile
	@$(ECHO) +++ $< : Create reference $@
	@$(MKDIR) -p output_reference
	@$(ECHO) $(ECHOFLAGS) "\tGBLL REFERENCE\nREFERENCE\tSETL\t{TRUE}\n\tEND" >config.h
	@$(AS) $(ASFLAGS) -o $@ $<
