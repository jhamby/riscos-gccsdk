# riscos-dist contains the supporting files that are only applicable
# to the RISC OS distribution of GCC.  Once the compiler has been built,
# this makefile will install the additional files to make a complete
# RISC OS distribution of GCC.
#
pkg=riscos-dist

bindir=$(pkg)/bin/$(TARGET_NAME)/$(VERSION)
libdir=$(pkg)/lib/gcc-lib/$(TARGET_NAME)/$(VERSION)

GCCPKG = $(pkg)/\!Boot,feb \
	$(pkg)/\!Run,feb \
	$(pkg)/\!Sprites,ff9 \
	$(pkg)/\!Sprites22,ff9 \
	$(pkg)/Changes \
	$(pkg)/support \
	$(pkg)/bin/drlink \
	$(pkg)/bin/dyn_kill,ffb \
	$(pkg)/bin/gcc \
	$(pkg)/bin/gcov \
	$(pkg)/bin/libfile \
	$(bindir)/as \
	$(bindir)/cpp \
	$(bindir)/ld \
	$(bindir)/apcs26/libscl/o/libscl \
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

CC1PKG = $(pkg)/bin/g++ \
	$(bindir)/cc1 \
	$(libdir)/include/h \
	$(bindir)/apcs26/unixlib/o/libiostream \
	$(bindir)/apcs26/unixlib/o/libstdc++

CC1PLUSPKG = $(bindir)/cc1plus \
	$(libdir)/include/g++-3

G77PKG = $(pkg)/bin/g77 \
	$(bindir)/f771 \
	$(bindir)/apcs26/unixlib/o/libg2c \
	$(pkg)/docs/g77 \
	$(pkg)/docs/g77-readme

GPCPKG = $(bindir)/gpc1 \
	$(bindir)/gpcpp \
	$(bindir)/apcs26/unixlib/o/libgpc \
	$(pkg)/docs/gpc

JAVAPKG = $(bindir)/jc1 \
	$(bindir)/jcf-dump \
	$(bindir)/jv-scan \
	$(bindir)/jvgenmain \
	$(bindir)/gcjh

UTILSPKG = $(pkg)/bin/bison \
	$(pkg)/bin/egrep \
	$(pkg)/bin/fgrep \
	$(pkg)/bin/grep \
	$(pkg)/bin/flex \
	$(pkg)/bin/gperf \
	$(pkg)/bin/gzip \
	$(pkg)/bin/make \
	$(pkg)/bin/mkdir \
	$(pkg)/bin/patch \
	$(pkg)/bin/sed \
	$(pkg)/bin/touch \
	$(bindir)/apcs26/unixlib/o/libfl \
	$(bindir)/apcs26/unixlib/o/libiberty \
	$(bindir)/apcs26/unixlib/o/libmp

all:	install

install:
	for dir in bin docs examples support sys; do \
		cp -a $$dir $(ro_gccpkg) ; done
	cp Boot,feb $(ro_gccpkg)/\!Boot,feb
	cp Run,feb $(ro_gccpkg)/\!Run,feb
	cp Sprites,ff9 $(ro_gccpkg)/\!Sprites,ff9
	cp Sprites22,ff9 $(ro_gccpkg)/\!Sprites22,ff9
	cp Changes $(ro_gccpkg)/Changes
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
	zip -9r $(ro_zippkg)/gcc.zip $(GCCPKG) ; \
	zip -9r $(ro_zippkg)/gccdoc.zip $(GCCDOCPKG) ; \
	zip -9r $(ro_zippkg)/cc1.zip $(CC1PKG) ; \
	zip -9r $(ro_zippkg)/cc1plus.zip $(CC1PLUSPKG) ; \
	zip -9r $(ro_zippkg)/g77.zip $(G77PKG) ; \
	zip -9r $(ro_zippkg)/gpc.zip $(GPCPKG) ; \
	zip -9r $(ro_zippkg)/java.zip $(JAVAPKG) ; \
	zip -9r $(ro_zippkg)/utils.zip $(UTILSPKG) ; )

