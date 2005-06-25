# riscos-dist contains the supporting files that are only applicable
# to the RISC OS distribution of GCC.  Once the compiler has been built,
# this makefile will install the additional files to make a complete
# RISC OS distribution of GCC.
#
pkg=`basename $(ro_gccpkg)`
upkg=`basename $(ro_unixlibpkg)`
spkg=`basename $(ro_libsclpkg)`

bindir=$(pkg)/bin/$(TARGET_NAME)/$(VERSION)
libdir=$(pkg)/lib/gcc-lib/$(TARGET_NAME)/$(VERSION)

GCCPKG = $(pkg)/\!Run,feb \
	$(pkg)/\!Sprites,ff9 \
	$(pkg)/\!Sprites22,ff9 \
	$(pkg)/\!Help \
	$(pkg)/Changes \
	$(pkg)/bin/ar$(exesfx) \
	$(pkg)/bin/decaof$(exesfx) \
	$(pkg)/bin/drlink$(exesfx) \
	$(pkg)/bin/dyn_kill,ffb \
	$(pkg)/bin/gcc$(exesfx) \
	$(pkg)/bin/gcov$(exesfx) \
	$(pkg)/bin/libfile$(exesfx) \
	$(pkg)/bin/nm$(exesfx) \
	$(pkg)/bin/rink$(exesfx) \
	$(pkg)/bin/rinkptr$(exesfx) \
	$(pkg)/include \
	$(pkg)/startup \
	$(bindir)/as$(exesfx) \
	$(bindir)/cpp$(exesfx) \
	$(bindir)/ld$(exesfx) \
	$(bindir)/cc1obj$(exesfx) \
	$(bindir)/apcs32/abs/libscl/o/libscl \
	$(bindir)/apcs32/abs/libscl/o/libgcc \
	$(bindir)/apcs32/abs/unixlib/o/libgcc \
	$(bindir)/apcs32/abs/unixlib/o/libobjc \
	$(bindir)/apcs32/abs/unixlib/o/unixlib \
	$(bindir)/apcs32/rma/libscl/o/libscl \
	$(bindir)/apcs32/rma/libscl/o/libgcc

GCCDOCPKG = $(pkg)/docs/COPYING \
	$(pkg)/docs/COPYINGLIB \
	$(pkg)/docs/Intro \
	$(pkg)/docs/bugs \
	$(pkg)/docs/cpp \
	$(pkg)/docs/diff \
	$(pkg)/docs/gcc \
	$(pkg)/docs/ULChanges \
	$(pkg)/docs/UnixLib \
	$(pkg)/examples

CC1PKG = $(pkg)/bin/g++$(exesfx) \
	$(bindir)/cc1$(exesfx) \
	$(libdir)/include/h \
	$(libdir)/include/objc

CC1PLUSPKG = \
	$(bindir)/cc1plus$(exesfx) \
	$(libdir)/include/g++-3 \
	$(bindir)/apcs32/abs/libscl/o/libstdc++ \
	$(bindir)/apcs32/abs/unixlib/o/libstdc++

G77PKG = $(pkg)/bin/g77$(exesfx) \
	$(bindir)/f771$(exesfx) \
	$(bindir)/apcs32/abs/unixlib/o/libg2c \
	$(bindir)/apcs32/abs/libscl/o/libg2c \
	$(pkg)/docs/g77 \
	$(pkg)/docs/g77-readme

UTILSPKG = $(pkg)/bin/bison$(exesfx) \
	$(pkg)/bin/cmp$(exesfx) \
	$(pkg)/bin/diff$(exesfx) \
	$(pkg)/bin/diff3$(exesfx) \
	$(pkg)/bin/extractsym$(exesfx) \
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
	$(pkg)/bin/zip$(exesfx) \
	$(pkg)/docs/bison \
	$(pkg)/docs/flex \
	$(pkg)/docs/grep \
	$(pkg)/docs/gperf \
	$(pkg)/docs/make \
	$(pkg)/docs/rink \
	$(pkg)/docs/sed \
	$(bindir)/apcs32/abs/unixlib/o/libfl \
	$(bindir)/apcs32/abs/unixlib/o/libgcov \
	$(bindir)/apcs32/abs/unixlib/o/libiberty \
	$(bindir)/apcs32/abs/libscl/o/libgcov

SYSTEMPKG = `basename $(ro_syspkg)`

UNIXLIBBINPKG = $(upkg)/\!Boot,feb \
	$(upkg)/\!Run,feb \
	$(upkg)/\!Sprites,ff9 \
	$(upkg)/\!Sprites22,ff9 \
	$(upkg)/\!Help \
	$(upkg)/Docs \
	$(upkg)/source/clib

UNIXLIBSRCPKG = $(upkg)/\!Boot,feb \
	$(upkg)/\!Run,feb \
	$(upkg)/\!Sprites,ff9 \
	$(upkg)/\!Sprites22,ff9 \
	$(upkg)/\!Help \
	$(upkg)/Docs \
	$(upkg)/objs \
	$(upkg)/source

LIBSCLPKG = $(spkg)

all:	install

install:
	for dir in bin docs examples; do \
		cp -pR $$dir $(ro_gccpkg) ; done
	cp Run,feb $(ro_gccpkg)/\!Run,feb
	cp Sprites,ff9 $(ro_gccpkg)/\!Sprites,ff9
	cp Sprites22,ff9 $(ro_gccpkg)/\!Sprites22,ff9
	cp Changes $(ro_gccpkg)/Changes
	cp Help $(ro_gccpkg)/\!Help
	find $(ro_gccpkg) -name "CVS" -type d | xargs rm -rf
	find $(ro_gccpkg) -name "*~*" -or -name "*.orig*" -or -name "*.reg*" -type f | xargs rm -rf
	cp ../unixlib/Docs/ReadMe $(ro_gccpkg)/docs/UnixLib
	cp ../unixlib/Docs/Changelog $(ro_gccpkg)/docs/ULChanges

	cp -pR ../unixlib/Docs $(ro_unixlibpkg)
	cp -pR ../unixlib/objs $(ro_unixlibpkg)
	cp ../unixlib/Boot,feb $(ro_unixlibpkg)/\!Boot,feb
	cp ../unixlib/\!Help $(ro_unixlibpkg)/\!Help
	cp ../unixlib/Run,feb $(ro_unixlibpkg)/\!Run,feb
	cp ../unixlib/Sprites,ff9 $(ro_unixlibpkg)/\!Sprites,ff9
	cp ../unixlib/Sprites22,ff9 $(ro_unixlibpkg)/\!Sprites22,ff9
	find $(ro_unixlibpkg) -name "CVS" -type d | xargs rm -rf
	find $(ro_unixlibpkg) -name "*~*" -or -name "*.orig*" -or -name "*.reg*" -type f | xargs rm -rf

	cp ../libscl/Boot,feb $(ro_libsclpkg)/\!Boot,feb
	cp ../libscl/Help $(ro_libsclpkg)/\!Help
	cp ../libscl/Run,feb $(ro_libsclpkg)/\!Run,feb
	cp ../libscl/Sprites,ff9 $(ro_libsclpkg)/\!Sprites,ff9
	cp ../libscl/Sprites22,ff9 $(ro_libsclpkg)/\!Sprites22,ff9

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
	rm -f $(ro_zippkg)/unixlibbin.zip; \
	rm -f $(ro_zippkg)/unixlibsrc.zip; \
	rm -f $(ro_zippkg)/libscl.zip; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/gcc.zip $(GCCPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/gccdoc.zip $(GCCDOCPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/cc1.zip $(CC1PKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/cc1plus.zip $(CC1PLUSPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/g77.zip $(G77PKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/utils.zip $(UTILSPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/system.zip $(SYSTEMPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/unixlibbin.zip $(UNIXLIBBINPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/unixlibsrc.zip $(UNIXLIBSRCPKG) ; \
	$(ux_bin_dir)/zip -, -9r $(ro_zippkg)/libscl.zip $(LIBSCLPKG) ; )

