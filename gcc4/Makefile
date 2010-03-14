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
NEWLIB_VERSION=1.18.0
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
GCC_CONFIG_ARGS += --with-pkgversion='GCCSDK GCC $(GCC_VERSION) Release 3 Development' \
	--with-bugurl=http://gccsdk.riscos.info/
BINUTILS_CONFIG_ARGS += --with-pkgversion='GCCSDK GCC $(GCC_VERSION) Release 3 Development' \
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
PREFIX_BUILDTOOL_BINUTILS := $(BUILDDIR)/installed-buildtools-for-binutils
PREFIX_BUILDTOOL_GCC := $(BUILDDIR)/installed-buildtools-for-gcc
SRCDIR := $(GCCSDK_SRCDIR)
SRCORIGDIR := $(GCCSDK_SRCDIR).orig
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
buildstepsdir/cross-done: buildstepsdir/cross-gcc \
	buildstepsdir/cross-riscostools
	touch buildstepsdir/cross-done

# Builds the native RISC OS compiler (gcc and binutils):
ronative: $(GCCSDK_INTERNAL_GETENV)
buildstepsdir/ronative-done: buildstepsdir/ronative-gcc \
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
	-rm -rf $(SRCORIGDIR) $(PREFIX_CROSS) $(PREFIX_RONATIVE)

# Respawn Makefile again after having loaded all our GCCSDK environment variables.
ifeq ($(GCCSDK_INTERNAL_GETENV),getenv)
getenv:
	@bash -c ". ./setup-gccsdk-params && $(MAKE) $(patsubst %,buildstepsdir/%-done,$(MAKECMDGOALS)) GCCSDK_INTERNAL_GETENV="
endif

# -- Configure & building:

# Configure & build autoconf-for-binutils tool:
buildstepsdir/buildtool-autoconf-for-binutils: buildstepsdir/src-autoconf-for-binutils
	-rm -rf $(BUILDDIR)/buildtool-autoconf-for-binutils
	mkdir -p $(BUILDDIR)/buildtool-autoconf-for-binutils
	cd $(BUILDDIR)/buildtool-autoconf-for-binutils && $(SRCDIR)/autoconf-for-binutils/configure --prefix=$(PREFIX_BUILDTOOL_BINUTILS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/buildtool-autoconf-for-binutils

# Configure & build autoconf-for-gcc tool:
buildstepsdir/buildtool-autoconf-for-gcc: buildstepsdir/src-autoconf-for-gcc
	-rm -rf $(BUILDDIR)/buildtool-autoconf-for-gcc
	mkdir -p $(BUILDDIR)/buildtool-autoconf-for-gcc
	cd $(BUILDDIR)/buildtool-autoconf-for-gcc && $(SRCDIR)/autoconf-for-gcc/configure --prefix=$(PREFIX_BUILDTOOL_GCC) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/buildtool-autoconf-for-gcc

# Configure & build automake-for-binutils tool:
buildstepsdir/buildtool-automake-for-binutils: buildstepsdir/src-automake-for-binutils
	-rm -rf $(BUILDDIR)/buildtool-automake-for-binutils
	mkdir -p $(BUILDDIR)/buildtool-automake-for-binutils
	cd $(BUILDDIR)/buildtool-automake-for-binutils && $(SRCDIR)/automake-for-binutils/configure --prefix=$(PREFIX_BUILDTOOL_BINUTILS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/buildtool-automake-for-binutils

# Configure & build automake-for-gcc tool:
buildstepsdir/buildtool-automake-for-gcc: buildstepsdir/src-automake-for-gcc
	-rm -rf $(BUILDDIR)/buildtool-automake-for-gcc
	mkdir -p $(BUILDDIR)/buildtool-automake-for-gcc
	cd $(BUILDDIR)/buildtool-automake-for-gcc && $(SRCDIR)/automake-for-gcc/configure --prefix=$(PREFIX_BUILDTOOL_GCC) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/buildtool-automake-for-gcc

# Configure & build binutils cross:
buildstepsdir/cross-binutils: buildstepsdir/src-binutils
	-rm -rf $(BUILDDIR)/cross-binutils
	mkdir -p $(BUILDDIR)/cross-binutils
	cd $(BUILDDIR)/cross-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SRCDIR)/binutils/configure $(CROSS_CONFIG_ARGS) $(BINUTILS_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/cross-binutils

# Configure & build binutils ronative:
buildstepsdir/ronative-binutils: buildstepsdir/src-binutils buildstepsdir/cross-done
	-rm -rf $(BUILDDIR)/ronative-binutils
	mkdir -p $(BUILDDIR)/ronative-binutils
	cd $(BUILDDIR)/ronative-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/binutils/configure $(RONATIVE_CONFIG_ARGS) $(BINUTILS_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/ronative-binutils

# Configure & build gcc cross:
buildstepsdir/cross-gcc: buildstepsdir/src-gcc buildstepsdir/cross-binutils
	-rm -rf $(BUILDDIR)/cross-gcc
	mkdir -p $(BUILDDIR)/cross-gcc
	cd $(BUILDDIR)/cross-gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gcc/configure $(CROSS_CONFIG_ARGS) $(GCC_CONFIG_ARGS) --enable-languages=$(GCC_LANGUAGES) && $(MAKE) $(GCC_BUILD_FLAGS) && $(MAKE) install
	touch buildstepsdir/cross-gcc

# Configure & build gcc ronative:
buildstepsdir/ronative-gcc: buildstepsdir/cross-done
	-rm -rf $(BUILDDIR)/ronative-gcc
	mkdir -p $(BUILDDIR)/ronative-gcc
	cd $(BUILDDIR)/ronative-gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gcc/configure $(RONATIVE_CONFIG_ARGS) $(GCC_CONFIG_ARGS) --enable-languages=$(GCC_LANGUAGES) && $(MAKE) $(GCC_BUILD_FLAGS) && $(MAKE) install
	touch buildstepsdir/ronative-gcc

# Configure & build gdb:
buildstepsdir/cross-gdb: buildstepsdir/src-gdb buildstepsdir/cross-gcc
	-rm -rf $(BUILDDIR)/cross-gdb
	mkdir -p $(BUILDDIR)/cross-gdb
	cd $(BUILDDIR)/cross-gdb && PATH="$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCORIGDIR)/gdb/configure $(CROSS_CONFIG_ARGS) $(GDB_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch buildstepsdir/cross-gdb

# Build the RISC OS related tools (cmunge, elf2aif, asasm, etc) cross:
buildstepsdir/cross-riscostools: buildstepsdir/cross-gcc
	cd $(RISCOSTOOLSDIR) && ./build-it cross
	touch buildstepsdir/cross-riscostools

# Build the RISC OS related tools (cmunge, elf2aif, asasm, etc) ronative:
buildstepsdir/ronative-riscostools: buildstepsdir/ronative-gcc
	cd $(RISCOSTOOLSDIR) && ./build-it riscos
	touch buildstepsdir/ronative-riscostools

# -- Source unpacking.

# Unpack autoconf-for-binutils source:
buildstepsdir/src-autoconf-for-binutils: $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION) $(SRCDIR)/autoconf-for-binutils
	cd $(SRCORIGDIR) && tar xfj autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/autoconf-for-binutils
	cp -T -p -r $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION) $(SRCDIR)/autoconf-for-binutils
	## cd $(SRCDIR)/autoconf-for-binutils && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-autoconf-for-binutils

# Unpack autoconf-for-gcc source:
buildstepsdir/src-autoconf-for-gcc: $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION) $(SRCDIR)/autoconf-for-gcc
	cd $(SRCORIGDIR) && tar xfj autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/autoconf-for-gcc
	cp -T -p -r $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION) $(SRCDIR)/autoconf-for-gcc
	## cd $(SRCDIR)/autoconf-for-gcc && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-autoconf-for-gcc

# Unpack automake-for-binutils source:
buildstepsdir/src-automake-for-binutils: $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION) $(SRCDIR)/automake-for-binutils
	cd $(SRCORIGDIR) && tar xfj automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/automake-for-binutils
	cp -T -p -r $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION) $(SRCDIR)/automake-for-binutils
	## cd $(SRCDIR)/automake-for-binutils && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-automake-for-binutils

# Unpack automake-for-gcc source:
buildstepsdir/src-automake-for-gcc: $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION) $(SRCDIR)/automake-for-gcc
	cd $(SRCORIGDIR) && tar xfj automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/automake-for-gcc
	cp -T -p -r $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION) $(SRCDIR)/automake-for-gcc
	## cd $(SRCDIR)/automake-for-gcc && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-automake-for-gcc

# Unpack binutils source:
buildstepsdir/src-binutils: $(SRCORIGDIR)/binutils-$(BINUTILS_VERSION).tar.bz2 buildstepsdir/buildtool-autoconf-for-binutils buildstepsdir/buildtool-automake-for-binutils
	-rm -rf $(SRCORIGDIR)/binutils-$(BINUTILS_VERSION) $(SRCDIR)/binutils
	cd $(SRCORIGDIR) && tar xfj binutils-$(BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/binutils
	cp -T -p -r $(SRCORIGDIR)/binutils-$(BINUTILS_VERSION) $(SRCDIR)/binutils
	cd $(SRCDIR)/binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-binutils

# Unpack gcc source:
buildstepsdir/src-gcc: $(SRCORIGDIR)/gcc-$(GCC_VERSION).tar.bz2 buildstepsdir/buildtool-autoconf-for-gcc buildstepsdir/buildtool-automake-for-gcc
	-rm -rf $(SRCORIGDIR)/gcc-$(GCC_VERSION) $(SRCDIR)/gcc
	cd $(SRCORIGDIR) && tar xfj gcc-$(GCC_VERSION).tar.bz2
	-mkdir -p $(SRCDIR)/gcc
	cp -T -p -r $(SRCORIGDIR)/gcc-$(GCC_VERSION) $(SRCDIR)/gcc
	cd $(SRCDIR)/gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-gcc

# Unpack newlib source:
buildstepsdir/src-newlib: $(SRCORIGDIR)/newlib-$(NEWLIB_VERSION).tar.gz
	-rm -rf $(SRCORIGDIR)/newlib-$(NEWLIB_VERSION) $(SRCDIR)/newlib
	cd $(SRCORIGDIR) && tar xfz newlib-$(NEWLIB_VERSION).tar.gz
	# FIXME: add stuff
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-newlib

# Unpack gdb source:
buildstepsdir/src-gdb: $(SRCORIGDIR)/gdb-$(GDB_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/gdb-$(GDB_VERSION) $(SRCDIR)/gdb
	cd $(SRCORIGDIR) && tar xfj gdb-$(GDB_VERSION).tar.bz2
	# FIXME: add stuff
	-mkdir -p buildstepsdir
	touch buildstepsdir/src-gdb

# -- Source downloading.

# Download autoconf source to be used to build binutils:
$(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2:
	-rm $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftp.gnu.org/gnu/autoconf/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2

# Download automake source to be used to build automake:
$(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2:
	-rm $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftp.gnu.org/gnu/automake/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2

ifneq ($(AUTOCONF_FOR_BINUTILS_VERSION),$(AUTOCONF_FOR_GCC_VERSION))
# Download autoconf source to be used to build gcc:
$(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2:
	-rm $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftp.gnu.org/gnu/autoconf/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
endif

ifneq ($(AUTOMAKE_FOR_BINUTILS_VERSION),$(AUTOMAKE_FOR_GCC_VERSION))
# Download automake source to be used to build gcc:
$(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2:
	-rm $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftp.gnu.org/gnu/automake/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
endif

# Download binutils source:
$(SRCORIGDIR)/binutils-$(BINUTILS_VERSION).tar.bz2:
	-rm $(SRCORIGDIR)/binutils-$(BINUTILS_VERSION).tar.bz2
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.bz2

# Download gcc source:
$(SRCORIGDIR)/gcc-$(GCC_VERSION).tar.bz2:
	-rm $(SRCORIGDIR)/gcc-$(GCC_VERSION).tar.bz2
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.bz2

# Download newlib source:
$(SRCORIGDIR)/newlib-$(NEWLIB_VERSION).tar.gz:
	-rm $(SRCORIGDIR)/newlib-$(NEWLIB_VERSION).tar.gz
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://sources.redhat.com/pub/newlib/newlib-$(NEWLIB_VERSION).tar.gz

# Download gdb source:
$(SRCORIGDIR)/gdb-$(GDB_VERSION).tar.bz2:
	-rm $(SRCORIGDIR)/gdb-$(GDB_VERSION).tar.bz2
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftp.gnu.org/gnu/gdb/gdb-$(GDB_VERSION).tar.bz2

