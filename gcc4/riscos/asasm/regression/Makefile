export HDR_PATH := $(CURDIR)/hdr
export APCS := APCS-32

AS = ../asasm
ASFLAGS = -t xscale -objasm
DIFF = diff
DIFFFLAGS = -q
ECHO = echo
ECHOFLAGS =

check_PROGRAMS = apcs arithmetic1 arithmetic2 branch builtinvar_endian \
	copro dsp fp info ldr_from_addr loadstore loadstore2 loadstore3 misc \
	register_based_map
ref_PROGRAMS = $(check_PROGRAMS:%=reference/%)

all: setup $(ref_PROGRAMS) $(check_PROGRAMS)
	@$(ECHO) All tests were OK

setup:
	@mkdir -p reference

clean:
	-rm -f $(check_PROGRAMS)
	-rm -f $(ref_PROGRAMS)
	-rm -f *.o
	-rm -f reference/*.o
	-rm -f config.h
	-rmdir reference

%: %.s Makefile
	@$(ECHO) $(ECHOFLAGS) "\tGBLL REFERENCE\nREFERENCE\tSETL\t{FALSE}\n\tEND" >config.h
	@$(AS) $(ASFLAGS) -o $@ $<
	@$(DIFF) $(DIFFFLAGS) $@ reference/$@ || ( rm $@ && false )

reference/%: %.s Makefile
	@$(ECHO) $(ECHOFLAGS) "\tGBLL REFERENCE\nREFERENCE\tSETL\t{TRUE}\n\tEND" >config.h
	@$(AS) $(ASFLAGS) -o $@ $<
