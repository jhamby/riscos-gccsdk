# riscos-dist contains the supporting files that are only applicable
# to the RISC OS distribution of GCC.  Once the compiler has been built,
# this makefile will install the additional files to make a complete
# RISC OS distribution of GCC.
#
pkg=`basename $(ro_gccpkg)`

bindir=$(pkg)/bin/$(TARGET_NAME)/$(VERSION)
libdir=$(pkg)/lib/gcc-lib/$(TARGET_NAME)/$(VERSION)

GCCPKG = $(pkg)/\!Boot,feb \
	$(pkg)/\!Run,feb \
	$(pkg)/\!Sprites,ff9 \
	$(pkg)/\!Sprites22,ff9 \
	$(pkg)/\!Help \
	$(pkg)/Changes \
	$(pkg)/include \
	$(pkg)/support \
	$(pkg)/bin/drlink$(exesfx) \
	$(pkg)/bin/dyn_kill,ffb \
	$(pkg)/bin/gcc$(exesfx) \
	$(pkg)/bin/gcov$(exesfx) \
	$(pkg)/bin/libfile$(exesfx) \
	$(bindir)/as$(exesfx) \
	$(bindir)/cpp$(exesfx) \
	$(bindir)/ld$(exesfx) \
	$(bindir)/apcs26/libscl/o/libscl \
	$(bindir)/apcs26/libscl/o/libgcc \
	$(bindir)/apcs26/unixlib/o/libgcc \
	$(bindir)/apcs26/unixlib/o/unixlib

GCCDOCPKG = $(pkg)/docs/COPYING \
	$(pkg)/docs/COPYINGLIB \
	$(pkg)/docs/Intro \
	$(pkg)/docs/bugs \
	$(pkg)/docs/gcc-faq \
	$(pkg)/docs/cpp \
	$(pkg)/docs/gcc \
	$(pkg)/examples

CC1PKG = $(pkg)/bin/g++$(exesfx) \
	$(bindir)/cc1$(exesfx) \
	$(libdir)/include/h \

CC1PLUSPKG = $(bindir)/cc1plus$(exesfx) \
	$(libdir)/include/g++-3 \
	$(bindir)/apcs26/unixlib/o/libiostream \
	$(bindir)/apcs26/unixlib/o/libio \
	$(bindir)/apcs26/unixlib/o/libstdc++

G77PKG = $(pkg)/bin/g77$(exesfx) \
	$(bindir)/f771$(exesfx) \
	$(bindir)/apcs26/unixlib/o/libg2c \
	$(pkg)/docs/g77 \
	$(pkg)/docs/g77-readme

GPCPKG = $(pkg)/bin/gpc$(exesfx) \
	$(bindir)/gpc1$(exesfx) \
	$(bindir)/gpcpp$(exesfx) \
	$(bindir)/apcs26/unixlib/o/libgpc \
	$(pkg)/docs/gpc

JAVAPKG = $(bindir)/jc1$(exesfx) \
	$(bindir)/jcf-dump$(exesfx) \
	$(bindir)/jv-scan$(exesfx) \
	$(bindir)/jvgenmain$(exesfx) \
	$(bindir)/gcjh$(exesfx)

UTILSPKG = $(pkg)/bin/bison$(exesfx) \
	$(pkg)/bin/cmp$(exesfx) \
	$(pkg)/bin/diff$(exesfx) \
	$(pkg)/bin/diff3$(exesfx) \
	$(pkg)/bin/egrep$(exesfx) \
	$(pkg)/bin/fgrep$(exesfx) \
	$(pkg)/bin/flex$(exesfx) \
	$(pkg)/bin/gperf$(exesfx) \
	$(pkg)/bin/grep$(exesfx) \
	$(pkg)/bin/gzip$(exesfx) \
	$(pkg)/bin/make$(exesfx) \
	$(pkg)/bin/mkdir$(exesfx) \
	$(pkg)/bin/patch$(exesfx) \
	$(pkg)/bin/sdiff$(exesfx) \
	$(pkg)/bin/sed$(exesfx) \
	$(pkg)/bin/touch$(exesfx) \
	$(bindir)/apcs26/unixlib/o/libfl \
	$(bindir)/apcs26/unixlib/o/libiberty \
	$(bindir)/apcs26/unixlib/o/libmp

SYSTEMPKG = `basename $(ro_syspkg)`

all:	install

install:
	for dir in bin docs examples support sys; do \
		cp -pR $$dir $(ro_gccpkg) ; done
	cp Boot,feb $(ro_gccpkg)/\!Boot,feb
	cp Run,feb $(ro_gccpkg)/\!Run,feb
	cp Sprites,ff9 $(ro_gccpkg)/\!Sprites,ff9
	cp Sprites22,ff9 $(ro_gccpkg)/\!Sprites22,ff9
	cp Changes $(ro_gccpkg)/Changes
	cp Help $(ro_gccpkg)/\!Help
	find $(ro_gccpkg) -name "CVS" -type d | xargs rm -rf


# Build the seperate RISC OS GCC zip packages
package:
	( cd $(ro_gccpkg)/../ ; \
	rm -f $(ro_zippkg)/gcc.zip; \
	rm -f $(ro_zippkg)/gccdoc.zip; \
	rm -f $(ro_zippkg)/cc1.zip; \
	rm -f $(ro_zippkg)/cc1plus.zip; \
	rm -f $(ro_zippkg)/g77.zip; \
	rm -f $(ro_zippkg)/gpc.zip; \
	rm -f $(ro_zippkg)/java.zip; \
	rm -f $(ro_zippkg)/utils.zip; \
	rm -f $(ro_zippkg)/system.zip; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/gcc.zip $(GCCPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/gccdoc.zip $(GCCDOCPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/cc1.zip $(CC1PKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/cc1plus.zip $(CC1PLUSPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/g77.zip $(G77PKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/gpc.zip $(GPCPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/java.zip $(JAVAPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/utils.zip $(UTILSPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/system.zip $(SYSTEMPKG) ; )

