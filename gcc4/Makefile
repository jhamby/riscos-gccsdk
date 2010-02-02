# Build GCCSDK
# Written by John Tytgat / BASS
#
# Needed:
#   - apt-get install flex bison libgmp3-dev libmpfr-dev texinfo build-essential
#   - binutils 2.17 & gcc 4.1 needs automake1.9.6 autoconf2.59
#     bintuils 2.20 & gcc 4.4 needs automake1.11 autoconf2.64
#     automake/autoconf will be fetched & built automatically.
#   - gdb requires libncurses5-dev

# TARGET can have following values: arm-unknown-riscos (stable), arm-unknown-eabi (work in progress)
TARGET=arm-unknown-riscos
GCC_LANGUAGES="c,c++"

AUTOCONF_FOR_BINUTILS_VERSION=2.64
AUTOMAKE_FOR_BINUTILS_VERSION=1.11
BINUTILS_VERSION=$(GCCSDK_SUPPORTED_BINUTILS_RELEASE)
AUTOCONF_FOR_GCC_VERSION=2.59
AUTOMAKE_FOR_GCC_VERSION=1.9.6
GCC_VERSION=$(GCCSDK_SUPPORTED_GCC_RELEASE)
NEWLIB_VERSION=1.17.0
GDB_VERSION=6.8

# Notes:
#   1) --with-cross-host is needed to correctly find the target libraries in
#      $GCCSDK_{CROSS|RISCOS}_PREFIX/arm-unknown-riscos/lib instead of
#      $GCCSDK_{CROSS|RISCOS}_PREFIX/lib.
#   2) Configure options are explained at http://gcc.gnu.org/install/configure.html
#      but note that this explanation is for the latest gcc version released and
#      not necessary the gcc version we're building here.
ifeq ($(TARGET),arm-unknown-riscos)
# Case GCCSDK arm-unknown-riscos target:
GCC_CONFIG_ARGS := \
	--enable-threads=posix \
	--enable-sjlj-exceptions=no \
	--enable-c99 \
	--enable-cmath \
	--enable-shared=libunixlib,libgcc,libstdc++ \
	--enable-multilib \
	--disable-c-mbchar \
	--disable-libstdcxx-pch \
	--disable-tls \
	--without-pic \
	--with-cross-host
# FIXME: for Java support: --without-x --enable-libgcj
BINUTILS_CONFIG_ARGS =
else
# Case arm-unknown-eabi target (use newlib):
GCC_CONFIG_ARGS := --disable-threads --disable-multilib --disable-shared --with-newlib
BINUTILS_CONFIG_ARGS := --disable-multilib --disable-shared
endif
# Note: --with-pkgversion & --with-bugurl is only supported from binutils 2.18 (?) & gcc 4.3 onwards.
GCC_CONFIG_ARGS += --with-pkgversion='GCCSDK GCC $(GCC_VERSION) Release 3 Alpha 1' \
	--with-bugurl=http://gccsdk.riscos.info/
BINUTILS_CONFIG_ARGS += --with-pkgversion='GCCSDK GCC $(GCC_VERSION) Release 3 Alpha 1' \
	--with-bugurl=http://gccsdk.riscos.info/
BINUTILS_CONFIG_ARGS += --enable-maintainer-mode --enable-interwork --disable-werror --with-gcc --disable-nls
GCC_CONFIG_ARGS += --enable-maintainer-mode --enable-interwork --disable-nls
NEWLIB_CONFIG_ARGS += --enable-interwork --disable-multilib --disable-shared --disable-nls
GDB_CONFIG_ARGS += --enable-interwork --disable-multilib --disable-werror --disable-nls

# When debugging/testing/validating the compiler add "-enable-checking=all",
# otherwise add "--enable-checking=release" or even "--enable-checking=no"
GCC_CONFIG_ARGS += --enable-checking=no
# Configure args shared between different targets:
# For debugging:
# FIXME: add to GCC_BUILD_FLAGS for optimized ARM libraries: CFLAGS_FOR_TARGET="-O3 -march=armv5" CXXFLAGS_FOR_TARGET="-O3 -march=armv5"
# Or perhaps better, at GCC configure time something like --with-arch=armv6 --with-tune=cortex-a8 --with-float=softfp --with-fpu=vfp ?
# GCC_BUILD_FLAGS = CFLAGS="-O0 -g" LIBCFLAGS="-O0 -g" LIBCXXFLAGS="-O0 -g"

ROOT := $(shell pwd)
PREFIX_CROSS := $(GCCSDK_CROSS_PREFIX)
PREFIX_RONATIVE := $(GCCSDK_RISCOS_PREFIX)/\!GCC
BUILDDIR := $(GCCSDK_BUILDDIR)
PREFIX_BUILDTOOL_BINUTILS := $(BUILDDIR)/cross-buildtool-binutils
PREFIX_BUILDTOOL_GCC := $(BUILDDIR)/cross-buildtool-gcc
SRCDIR := $(GCCSDK_SRCDIR)
ORIGSRCDIR := $(GCCSDK_SRCDIR).orig
SCRIPTSDIR := $(ROOT)/scripts
RECIPEDIR := $(ROOT)/recipe
RISCOSTOOLSDIR := $(GCCSDK_RISCOS)

# Configure args unique for cross-compiling & unique to building for RISC OS native
CROSS_CONFIG_ARGS := --target=$(TARGET) --prefix=$(PREFIX_CROSS)
# Note: --build argument can only be determined when SRCDIR is populated.
RONATIVE_CONFIG_ARGS = --build=`$(SRCDIR)/gcc/config.guess` --host=$(TARGET) --target=$(TARGET) --prefix=$(PREFIX_RONATIVE)

# To respawn Makefile with setup-gccsdk-param environment loaded.
GCCSDK_INTERNAL_GETENV=getenv
ifeq ($(MAKECMDGOALS),)
MAKECMDGOALS=all
endif

.NOTPARALLEL:
.PHONY: all cross ronative clean distclean

# Default target is to build the cross-compiler (including the RISC OS tools):
all: $(GCCSDK_INTERNAL_GETENV)
buildstepsdir/all-done: buildstepsdir/cross-done

# Builds the cross compiler:
cross: $(GCCSDK_INTERNAL_GETENV)
buildstepsdir/cross-done: buildstepsdir/cross-gcc-full \
	buildstepsdir/cross-riscostools
	touch buildstepsdir/cross-done

# Builds the native RISC OS compiler (gcc and binutils):
ronative: $(GCCSDK_INTERNAL_GETENV)
buildstepsdir/ronative-done: buildstepsdir/ronative-gcc-full \
	buildstepsdir/ronative-binutils \
	buildstepsdir/ronative-riscostools
	touch buildstepsdir/ronative-done

clean: $(GCCSDK_INTERNAL_GETENV)
buildstepsdir/clean-done:
	-rm -rf $(BUILDDIR)
	-rm -rf $(SRCDIR)
	-rm -rf buildstepsdir

distclean: $(GCCSDK_INTERNAL_GETENV)
buildstepsdir/distclean-done:
	-rm -rf $(BUILDDIR)
	-rm -rf $(SRCDIR)
	-rm -rf buildstepsdir
	-rm -rf $(ORIGSRCDIR) $(PREFIX_CROSS) $(PREFIX_RONATIVE)

# Respawn Makefile again after having loaded all our GCCSDK environment variables.
ifeq ($(GCCSDK_INTERNAL_GETENV),getenv)
getenv:
	@bash -c ". ./setup-gccsdk-params && $(MAKE) $(patsubst %,buildstepsdir/%-done,$(MAKECMDGOALS)) GCCSDK_INTERNAL_GETENV="
endif

# -- Building

# Build autoconf-for-binutils tool:
buildstepsdir/tool-autoconf-for-binutils: buildstepsdir/src-autoconf-for-binutils
	-rm -rf $(BUILDDIR)/tool-autoconf-for-binutils
	mkdir -p $(BUILDDIR)/tool-autoconf-for-binutils
	cd $(BUILDDIR)/tool-autoconf-for-binutils && $(SRCDIR)/autoconf-for-binutils/configure --prefix=$(PREFIX_BUILDTOOL_BINUTILS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/tool-autoconf-for-binutils

# Build automake-for-binutils tool:
buildstepsdir/tool-automake-for-binutils: buildstepsdir/src-automake-for-binutils
	-rm -rf $(BUILDDIR)/tool-automake-for-binutils
	mkdir -p $(BUILDDIR)/tool-automake-for-binutils
	cd $(BUILDDIR)/tool-automake-for-binutils && $(SRCDIR)/automake-for-binutils/configure --prefix=$(PREFIX_BUILDTOOL_BINUTILS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/tool-automake-for-binutils

# Build binutils cross:
buildstepsdir/cross-binutils: buildstepsdir/src-binutils
	-rm -rf $(BUILDDIR)/cross-binutils
	mkdir -p $(BUILDDIR)/cross-binutils
	cd $(BUILDDIR)/cross-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SRCDIR)/binutils/configure $(CROSS_CONFIG_ARGS) $(BINUTILS_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/cross-binutils

# Build binutils ronative:
buildstepsdir/ronative-binutils: buildstepsdir/src-binutils buildstepsdir/cross-done
	-rm -rf $(BUILDDIR)/ronative-binutils
	mkdir -p $(BUILDDIR)/ronative-binutils
	cd $(BUILDDIR)/ronative-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/binutils/configure $(RONATIVE_CONFIG_ARGS) $(BINUTILS_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/ronative-binutils

# Build autoconf-for-gcc tool:
buildstepsdir/tool-autoconf-for-gcc: buildstepsdir/src-autoconf-for-gcc
	-rm -rf $(BUILDDIR)/tool-autoconf-for-gcc
	mkdir -p $(BUILDDIR)/tool-autoconf-for-gcc
	cd $(BUILDDIR)/tool-autoconf-for-gcc && $(SRCDIR)/autoconf-for-gcc/configure --prefix=$(PREFIX_BUILDTOOL_GCC) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/tool-autoconf-for-gcc

# Build automake-for-gcc tool:
buildstepsdir/tool-automake-for-gcc: buildstepsdir/src-automake-for-gcc
	-rm -rf $(BUILDDIR)/tool-automake-for-gcc
	mkdir -p $(BUILDDIR)/tool-automake-for-gcc
	cd $(BUILDDIR)/tool-automake-for-gcc && $(SRCDIR)/automake-for-gcc/configure --prefix=$(PREFIX_BUILDTOOL_GCC) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/tool-automake-for-gcc

# Build minimal (bootstrap) gcc cross (without target runtime support):
ifneq ($(TARGET),arm-unknown-riscos)
buildstepsdir/cross-gcc-bootstrap: buildstepsdir/src-gcc buildstepsdir/cross-binutils
	-rm -rf $(BUILDDIR)/cross-gcc-bootstrap
	mkdir -p $(BUILDDIR)/cross-gcc-bootstrap
	cd $(BUILDDIR)/cross-gcc-bootstrap && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gcc/configure $(CROSS_CONFIG_ARGS) $(GCC_CONFIG_ARGS) --enable-languages="c" --without-headers && $(MAKE) all-gcc && $(MAKE) install-gcc
	touch buildstepsdir/cross-gcc-bootstrap
else
buildstepsdir/cross-gcc-bootstrap: buildstepsdir/src-gcc buildstepsdir/cross-binutils
	-rm -rf $(BUILDDIR)/cross-gcc-bootstrap
	touch buildstepsdir/cross-gcc-bootstrap
endif

# Build full gcc cross (we need target runtime support by now):
ifneq ($(TARGET),arm-unknown-riscos)
buildstepsdir/cross-gcc-full: buildstepsdir/cross-newlib
endif
buildstepsdir/cross-gcc-full: buildstepsdir/cross-gcc-bootstrap
	-rm -rf $(BUILDDIR)/cross-gcc-full
	mkdir -p $(BUILDDIR)/cross-gcc-full
	cd $(BUILDDIR)/cross-gcc-full && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gcc/configure $(CROSS_CONFIG_ARGS) $(GCC_CONFIG_ARGS) --enable-languages=$(GCC_LANGUAGES) && $(MAKE) $(GCC_BUILD_FLAGS) && $(MAKE) install
	touch buildstepsdir/cross-gcc-full

# Build full gcc ronative (we need target runtime support by now):
ifneq ($(TARGET),arm-unknown-riscos)
buildstepsdir/ronative-gcc-full: buildstepsdir/ronative-newlib
endif
buildstepsdir/ronative-gcc-full: buildstepsdir/cross-done
	-rm -rf $(BUILDDIR)/ronative-gcc-full
	mkdir -p $(BUILDDIR)/ronative-gcc-full
	cd $(BUILDDIR)/ronative-gcc-full && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gcc/configure $(RONATIVE_CONFIG_ARGS) $(GCC_CONFIG_ARGS) --enable-languages=$(GCC_LANGUAGES) && $(MAKE) $(GCC_BUILD_FLAGS) && $(MAKE) install
	touch buildstepsdir/ronative-gcc-full

# Build newlib with minimal gcc:
buildstepsdir/cross-newlib: buildstepsdir/src-newlib buildstepsdir/cross-gcc-bootstrap
	-rm -rf $(BUILDDIR)/cross-newlib
	mkdir -p $(BUILDDIR)/cross-newlib
	cd $(BUILDDIR)/cross-newlib && PATH="$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/newlib/configure $(CROSS_CONFIG_ARGS) $(NEWLIB_CONFIG_ARGS) --enable-languages=$(GCC_LANGUAGES) && $(MAKE) $(GCC_BUILD_FLAGS) && $(MAKE) install
	touch buildstepsdir/cross-newlib

# Build gdb:
buildstepsdir/cross-gdb: buildstepsdir/src-gdb buildstepsdir/cross-gcc-full
	-rm -rf $(BUILDDIR)/cross-gdb
	mkdir -p $(BUILDDIR)/cross-gdb
	cd $(BUILDDIR)/cross-gdb && PATH="$(PREFIX_CROSS)/bin:$(PATH)" && $(ORIGSRCDIR)/gdb/configure $(CROSS_CONFIG_ARGS) $(GDB_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/cross-gdb

# Build the RISC OS related tools (cmunge, elf2aif, asasm, etc) cross:
buildstepsdir/cross-riscostools: buildstepsdir/cross-gcc-full
	cd $(RISCOSTOOLSDIR) && ./build-it cross
	touch buildstepsdir/cross-riscostools

# Build the RISC OS related tools (cmunge, elf2aif, asasm, etc) ronative:
buildstepsdir/ronative-riscostools: buildstepsdir/ronative-gcc-full
	cd $(RISCOSTOOLSDIR) && ./build-it riscos
	touch buildstepsdir/ronative-riscostools

# -- Source unpacking.

# Unpack autoconf-for-binutils source:
buildstepsdir/src-autoconf-for-binutils: $(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2
	-rm -rf $(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION) $(SRCDIR)/autoconf
	cd $(ORIGSRCDIR) && tar xfj autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/autoconf-for-binutils
	cp -T -p -r $(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION) $(SRCDIR)/autoconf-for-binutils
	## cd $(SRCDIR)/autoconf-for-binutils && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-autoconf-for-binutils

# Unpack automake-for-binutils source:
buildstepsdir/src-automake-for-binutils: $(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2
	-rm -rf $(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION) $(SRCDIR)/automake
	cd $(ORIGSRCDIR) && tar xfj automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/automake-for-binutils
	cp -T -p -r $(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION) $(SRCDIR)/automake-for-binutils
	## cd $(SRCDIR)/automake-for-binutils && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-automake-for-binutils

# Unpack binutils source:
buildstepsdir/src-binutils: $(ORIGSRCDIR)/binutils-$(BINUTILS_VERSION).tar.bz2 buildstepsdir/tool-autoconf-for-binutils buildstepsdir/tool-automake-for-binutils
	-rm -rf $(ORIGSRCDIR)/binutils-$(BINUTILS_VERSION) $(SRCDIR)/binutils
	cd $(ORIGSRCDIR) && tar xfj binutils-$(BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/binutils
	cp -T -p -r $(ORIGSRCDIR)/binutils-$(BINUTILS_VERSION) $(SRCDIR)/binutils
	cd $(SRCDIR)/binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-binutils

# Unpack autoconf-for-binutils source:
buildstepsdir/src-autoconf-for-gcc: $(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
	-rm -rf $(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION) $(SRCDIR)/autoconf
	cd $(ORIGSRCDIR) && tar xfj autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/autoconf-for-gcc
	cp -T -p -r $(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION) $(SRCDIR)/autoconf-for-gcc
	## cd $(SRCDIR)/autoconf-for-gcc && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-autoconf-for-gcc

# Unpack automake-for-gcc source:
buildstepsdir/src-automake-for-gcc: $(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
	-rm -rf $(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION) $(SRCDIR)/automake
	cd $(ORIGSRCDIR) && tar xfj automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/automake-for-gcc
	cp -T -p -r $(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION) $(SRCDIR)/automake-for-gcc
	## cd $(SRCDIR)/automake-for-gcc && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-automake-for-gcc

# Unpack gcc source:
buildstepsdir/src-gcc: $(ORIGSRCDIR)/gcc-$(GCC_VERSION).tar.bz2 buildstepsdir/tool-autoconf-for-gcc buildstepsdir/tool-automake-for-gcc
	-rm -rf $(ORIGSRCDIR)/gcc-$(GCC_VERSION) $(SRCDIR)/gcc
	cd $(ORIGSRCDIR) && tar xfj gcc-$(GCC_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/gcc
	cp -T -p -r $(ORIGSRCDIR)/gcc-$(GCC_VERSION) $(SRCDIR)/gcc
	cd $(SRCDIR)/gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-gcc

# Unpack newlib source:
buildstepsdir/src-newlib: $(ORIGSRCDIR)/newlib-$(NEWLIB_VERSION).tar.gz
	-rm -rf $(ORIGSRCDIR)/newlib-$(NEWLIB_VERSION) $(SRCDIR)/newlib
	cd $(ORIGSRCDIR) && tar xfz newlib-$(NEWLIB_VERSION).tar.gz
	# FIXME: add stuff
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-newlib

# Unpack gdb source:
buildstepsdir/src-gdb: $(ORIGSRCDIR)/gdb-$(GDB_VERSION).tar.bz2
	-rm -rf $(ORIGSRCDIR)/gdb-$(GDB_VERSION) $(SRCDIR)/gdb
	cd $(ORIGSRCDIR) && tar xfj gdb-$(GDB_VERSION).tar.bz2
	# FIXME: add stuff
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-gdb

# -- Source downloading.

# Download autoconf source to be used to build binutils:
$(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2:
	-rm $(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2
	-mkdir -p $(ORIGSRCDIR)
	cd $(ORIGSRCDIR) && wget -c http://ftp.gnu.org/gnu/autoconf/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2

# Download automake source to be used to build automake:
$(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2:
	-rm $(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2
	-mkdir -p $(ORIGSRCDIR)
	cd $(ORIGSRCDIR) && wget -c http://ftp.gnu.org/gnu/automake/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2

ifneq ($(AUTOCONF_FOR_BINUTILS_VERSION),$(AUTOCONF_FOR_GCC_VERSION))
# Download autoconf source to be used to build gcc:
$(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2:
	-rm $(ORIGSRCDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
	-mkdir -p $(ORIGSRCDIR)
	cd $(ORIGSRCDIR) && wget -c http://ftp.gnu.org/gnu/autoconf/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
endif

ifneq ($(AUTOMAKE_FOR_BINUTILS_VERSION),$(AUTOMAKE_FOR_GCC_VERSION))
# Download automake source to be used to build gcc:
$(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2:
	-rm $(ORIGSRCDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
	-mkdir -p $(ORIGSRCDIR)
	cd $(ORIGSRCDIR) && wget -c http://ftp.gnu.org/gnu/automake/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
endif

# Download binutils source:
$(ORIGSRCDIR)/binutils-$(BINUTILS_VERSION).tar.bz2:
	-rm $(ORIGSRCDIR)/binutils-$(BINUTILS_VERSION).tar.bz2
	-mkdir -p $(ORIGSRCDIR)
	cd $(ORIGSRCDIR) && wget -c http://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.bz2

# Download gcc source:
$(ORIGSRCDIR)/gcc-$(GCC_VERSION).tar.bz2:
	-rm $(ORIGSRCDIR)/gcc-$(GCC_VERSION).tar.bz2
	-mkdir -p $(ORIGSRCDIR)
	cd $(ORIGSRCDIR) && wget -c http://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.bz2

# Download newlib source:
$(ORIGSRCDIR)/newlib-$(NEWLIB_VERSION).tar.gz:
	-rm $(ORIGSRCDIR)/newlib-$(NEWLIB_VERSION).tar.gz
	-mkdir -p $(ORIGSRCDIR)
	cd $(ORIGSRCDIR) && wget -c http://sources.redhat.com/pub/newlib/newlib-$(NEWLIB_VERSION).tar.gz

# Download gdb source:
$(ORIGSRCDIR)/gdb-$(GDB_VERSION).tar.bz2:
	-rm $(ORIGSRCDIR)/gdb-$(GDB_VERSION).tar.bz2
	-mkdir -p $(ORIGSRCDIR)
	cd $(ORIGSRCDIR) && wget -c http://ftp.gnu.org/gnu/gdb/gdb-$(GDB_VERSION).tar.bz2

