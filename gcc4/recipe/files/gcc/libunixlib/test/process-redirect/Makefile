# Custom makefile to build different child flavours.

CC = $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-gcc
ELF2AIF = $(GCCSDK_INSTALL_CROSSBIN)/elf2aif

EXES = child_ul_elf,e1f child_ul_abs,ff8 child_scl,ff8 \
	parent_ul,e1f

all: $(EXES)

clean:
	-$(RM) $(EXES)

child_ul_elf,e1f: child.c
	$(CC) -static -o $@ $<

child_ul_abs,ff8: child.c
	$(CC) -static -o child_ul_abs_tmp,e1f $<
	$(ELF2AIF) child_ul_abs_tmp,e1f child_ul_abs,ff8
	$(RM) child_ul_abs_tmp,e1f

child_scl,ff8: child.c
	$(CC) -mlibscl -o $@ $<

parent_ul,e1f: parent.c
	$(CC) -static -o $@ $<

