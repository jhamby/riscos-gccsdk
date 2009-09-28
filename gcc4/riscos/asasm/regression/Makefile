export HDR_PATH := $(CURDIR)/hdr
export APCS := APCS-32

AS = asasm
ASFLAGS = -t xscale -objasm -aof
DIFF = diff
DIFFFLAGS = -q
ECHO = echo
ECHOFLAGS =

check_PROGRAMS_PASS := $(wildcard tests_pass/*.s)
check_PROGRAMS_FAIL := $(wildcard tests_fail/*.s)
ref_PROGRAMS = $(check_PROGRAMS_PASS:tests_pass/%.s=output_reference/%.o) $(check_PROGRAMS_FAIL:tests_fail/%.s=output_reference/%.o)
# We only verify the tests which are supposed to PASS, i.e. to detect regression problems.
# In order to execute the tests which are known to FAIL, use:
#   $ make output_actual/<failing_test>.o
act_PROGRAMS = $(check_PROGRAMS_PASS:tests_pass/%.s=output_actual/%.o)

VPATH = tests_pass:tests_fail

all: setup $(ref_PROGRAMS) $(act_PROGRAMS)
	@$(ECHO) All tests were OK

setup:
	@mkdir -p output_reference output_actual

clean:
	-rm -fr output_actual
	-rm -fr output_reference
	-rm -f config.h

output_actual/%.o: %.s output_reference/%.o Makefile
	@$(ECHO) +++ Assemble actual and compare with reference $<
	@$(ECHO) $(ECHOFLAGS) "\tGBLL REFERENCE\nREFERENCE\tSETL\t{FALSE}\n\tEND" >config.h
	@$(AS) $(ASFLAGS) -o $@ $<
	@$(DIFF) $(DIFFFLAGS) $@ $(@:output_actual/%=output_reference/%) || (rm $@ && false)

output_reference/%.o: %.s Makefile
	@$(ECHO) +++ Create reference $<
	@$(ECHO) $(ECHOFLAGS) "\tGBLL REFERENCE\nREFERENCE\tSETL\t{TRUE}\n\tEND" >config.h
	@$(AS) $(ASFLAGS) -o $@ $<
