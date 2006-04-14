# riscos-dist contains the supporting files that are only applicable
# to the RISC OS distribution of GCC.  Once the compiler has been built,
# this makefile will install the additional files to make a complete
# RISC OS distribution of GCC.
# Copyright (c) 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005, 2006 GCCSDK Developers

pkg=`basename $(ro_gccpkg)`

bindir=$(pkg)/bin/$(TARGET_NAME)/$(VERSION)
libdir=$(pkg)/lib/gcc-lib/$(TARGET_NAME)/$(VERSION)

GCCPKG = $(pkg)/\!Run,feb \
	$(pkg)/\!Sprites,ff9 \
	$(pkg)/\!Sprites22,ff9 \
	$(pkg)/\!Help \
	$(pkg)/Changes \
	$(pkg)/bin/ar$(exesfx) \
	$(pkg)/bin/decaof$(exesfx) \
	$(pkg)/docs/decaof/* \
	$(pkg)/bin/resgen$(exesfx) \
	$(pkg)/docs/resgen/* \
	$(pkg)/bin/drlink$(exesfx) \
	$(pkg)/docs/Drlink/* \
	$(pkg)/bin/gcc$(exesfx) \
	$(pkg)/bin/gcov$(exesfx) \
	$(pkg)/bin/libfile$(exesfx) \
	$(pkg)/docs/LibFile/* \
	$(pkg)/bin/nm$(exesfx) \
	$(pkg)/include \
	$(pkg)/startup \
	$(bindir)/as$(exesfx) \
	$(bindir)/cpp$(exesfx) \
	$(bindir)/ld$(exesfx) \
	$(bindir)/apcs32/abs/libscl/o/libscl \
	$(bindir)/apcs32/abs/libscl/o/libgcc \
	$(bindir)/apcs32/abs/unixlib/o/libgcc \
	$(bindir)/apcs32/abs/unixlib/o/libgcov \
	$(bindir)/apcs32/abs/unixlib/o/unixlib \
	$(bindir)/apcs32/rma/libscl/o/libscl \
	$(bindir)/apcs32/rma/libscl/o/libgcc \
	$(bindir)/apcs32/abs/libscl/o/libgcov

GCCDOCPKG = $(pkg)/docs/COPYING \
	$(pkg)/docs/COPYINGLIB \
	$(pkg)/docs/Intro \
	$(pkg)/docs/bugs \
	$(pkg)/docs/cpp \
	$(pkg)/docs/gcc \
	$(pkg)/docs/UnixLib/Changes \
	$(pkg)/docs/UnixLib/Copyright \
	$(pkg)/docs/UnixLib/UnixLib \
	$(pkg)/docs/DRenderer/COPYING \
	$(pkg)/docs/DRenderer/DocDRender \
	$(pkg)/examples

CC1PKG = $(pkg)/bin/g++$(exesfx) \
	$(bindir)/cc1$(exesfx) \
	$(libdir)/include/h

CC1PLUSPKG = \
	$(bindir)/cc1plus$(exesfx) \
	$(libdir)/include/g++-3 \
	$(bindir)/apcs32/abs/libscl/o/libstdc++ \
	$(bindir)/apcs32/abs/unixlib/o/libstdc++ \
	$(bindir)/apcs32/rma/libscl/o/libstdc++

G77PKG = $(pkg)/bin/g77$(exesfx) \
	$(bindir)/f771$(exesfx) \
	$(bindir)/apcs32/abs/unixlib/o/libg2c \
	$(bindir)/apcs32/abs/libscl/o/libg2c \
	$(pkg)/docs/g77 \
	$(pkg)/docs/g77-readme

CMUNGEPKG = $(pkg)/bin/cmunge$(exesfx) \
	$(pkg)/docs/CMunge/*

UTILSPKG = $(pkg)/bin/bison$(exesfx) \
	$(pkg)/docs/bison,faf \
	\
	$(pkg)/bin/cmp$(exesfx) \
	$(pkg)/bin/diff$(exesfx) \
	$(pkg)/bin/diff3$(exesfx) \
	$(pkg)/bin/sdiff$(exesfx) \
	$(pkg)/docs/diff,faf \
	\
	$(pkg)/bin/flex$(exesfx) \
	$(pkg)/docs/flex,faf \
	\
	$(pkg)/bin/gperf$(exesfx) \
	$(pkg)/docs/gperf \
	\
	$(pkg)/bin/grep$(exesfx) \
	$(pkg)/docs/grep,faf \
	\
	$(pkg)/bin/gzip$(exesfx) \
	$(pkg)/bin/make$(exesfx) \
	$(pkg)/docs/make \
	\
	$(pkg)/bin/mkdir$(exesfx) \
	$(pkg)/bin/patch$(exesfx) \
	$(pkg)/bin/sed$(exesfx) \
	$(pkg)/docs/sed,faf \
	\
	$(pkg)/bin/touch$(exesfx) \
	$(pkg)/bin/zip$(exesfx) \
	$(pkg)/docs/InfoZip/* \
	$(bindir)/apcs32/abs/unixlib/o/libfl \
	$(bindir)/apcs32/abs/unixlib/o/libiberty

SYSTEMPKG = `basename $(ro_syspkg)`

all:	install

install:
	for dir in $$( find . \! -path "*.svn*" -type d ) ; do mkdir -p $(ro_gccpkg)/$$dir ; find $$dir -maxdepth 1 -type f -exec cp -p {} $(ro_gccpkg)/$$dir \; ; done
	find $(ro_gccpkg) -name "*~*" -or -name "*.orig*" -or -name "*.reg*" -or -name Makefile -type f | xargs rm -rf
	
# Build the separate RISC OS GCC zip packages
package:
	( cd $(ro_gccpkg)/../ ; \
	rm -f $(ro_zippkg)/gcc.zip; \
	rm -f $(ro_zippkg)/gccdoc.zip; \
	rm -f $(ro_zippkg)/cc1.zip; \
	rm -f $(ro_zippkg)/cc1plus.zip; \
	rm -f $(ro_zippkg)/g77.zip; \
	rm -f $(ro_zippkg)/utils.zip; \
	rm -f $(ro_zippkg)/system.zip; \
	rm -f $(ro_zippkg)/cmunge.zip; \
	$(ux_bin_dir)/zip -, -9rq $(ro_zippkg)/gcc.zip $(GCCPKG) ; \
	$(ux_bin_dir)/zip -, -9rq $(ro_zippkg)/gccdoc.zip $(GCCDOCPKG) ; \
	$(ux_bin_dir)/zip -, -9rq $(ro_zippkg)/cc1.zip $(CC1PKG) ; \
	$(ux_bin_dir)/zip -, -9rq $(ro_zippkg)/cc1plus.zip $(CC1PLUSPKG) ; \
	$(ux_bin_dir)/zip -, -9rq $(ro_zippkg)/g77.zip $(G77PKG) ; \
	$(ux_bin_dir)/zip -, -9rq $(ro_zippkg)/utils.zip $(UTILSPKG) ; \
	$(ux_bin_dir)/zip -, -9rq $(ro_zippkg)/system.zip $(SYSTEMPKG) ; \
	$(ux_bin_dir)/zip -, -9rq $(ro_zippkg)/cmunge.zip $(CMUNGEPKG) ; )
