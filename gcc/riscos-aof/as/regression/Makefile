AS = $(GCCSDK_INSTALL_CROSSBIN)/gcc
ASFLAGS = -c -mcpu=xscale
DIFF = diff
DIFFFLAGS = -q
ECHO = echo
ECHOFLAGS = -e
LD = $(GCCSDK_INSTALL_CROSSBIN)/drlink
LDFLAGS = -bin

check_PROGRAMS = arithmetic1 arithmetic2 branch copro dsp fp loadstore loadstore2 loadstore3 misc
ref_PROGRAMS = $(check_PROGRAMS:%=reference/%)

all: setup $(ref_PROGRAMS) $(check_PROGRAMS)

setup:
	@mkdir -p reference

clean:
	-rm -f $(check_PROGRAMS)
	-rm -f $(ref_PROGRAMS)
	-rm -f *.o
	-rm -f reference/*.o
	-rm -f config.h
	-rmdir reference

%: %.s
	@$(ECHO) $(ECHOFLAGS) "\tGBLL REFERENCE\nREFERENCE\tSETL\t{FALSE}\n\tEND" >config.h
	@$(AS) $(ASFLAGS) -o $@.o $<
	@$(LD) $(LDFLAGS) -o $@ $@.o >/dev/null
	@$(DIFF) $(DIFFFLAGS) $@ reference/$@

reference/%: %.s
	@$(ECHO) $(ECHOFLAGS) "\tGBLL REFERENCE\nREFERENCE\tSETL\t{TRUE}\n\tEND" >config.h
	@$(AS) $(ASFLAGS) -o $@.o $<
	@$(LD) $(LDFLAGS) -o $@ $@.o >/dev/null
