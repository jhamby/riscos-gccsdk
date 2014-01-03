# To build GCCSDK cross-compiler and RISC OS native compiler.
# Written by John Tytgat / BASS
# Copyright (c) 2009-2014 GCCSDK Developers
#
# Build requirements:
#   - apt-get install flex bison libgmp3-dev libmpfr-dev texinfo build-essential
#   - binutils 2.24 and gcc 4.7 needs automake 1.11.1 and autoconf 2.64 and
#     a recent libtool version.
#     Note the required automake/autoconf/libtool sources will be fetched
#     & built automatically.
#   - gcc 4.7 needs at least GMP 4.3.2, MPFR 2.4.2 and MPC 0.8.1 (MPFR needs GMP, MPC needs GMP & MPFR)
#     For MPFR/GMP/MPC version numbers, a good set can be found mentioned at gcc/contrib/download_prerequisites.
#   - gdb requires libncurses5-dev

# TARGET can have following values: arm-unknown-riscos (stable), arm-unknown-eabi (work in progress)
TARGET=arm-unknown-riscos
# TARGET=arm-unknown-eabi
GCC_LANGUAGES="c,c++"
# Enable shared library support in the cross/ronative compiler ?
CROSS_ENABLE_SHARED=yes
RONATIVE_ENABLE_SHARED=yes

# *_SCM variables: when set to "yes", we'll fetch the source from source control
# system and it will always be the latest version.
# GCC_USE_PPL_CLOOG: when set to "yes", this enables additional loop optimisation.
# This requires a C++ compiler (e.g. when building the RISC OS native compiler, this
# option requires a working C++ cross compiler).
AUTOCONF_FOR_BINUTILS_VERSION=2.64
AUTOMAKE_FOR_BINUTILS_VERSION=1.11.1
LIBTOOL_FOR_BINUTILS_VERSION=2.4.2
BINUTILS_VERSION=$(GCCSDK_SUPPORTED_BINUTILS_RELEASE)
AUTOCONF_FOR_GCC_VERSION=2.64
AUTOMAKE_FOR_GCC_VERSION=1.11.1
LIBTOOL_FOR_GCC_VERSION=2.4.2
GCC_VERSION=$(GCCSDK_SUPPORTED_GCC_RELEASE)
GCC_USE_SCM=yes
GCC_SCM_PATH=branches/gcc-4_7-branch
NEWLIB_VERSION=1.19.0
NEWLIB_USE_SCM=no
GDB_VERSION=7.2
GMP_VERSION=5.0.1
MPFR_VERSION=3.0.1
MPC_VERSION=0.8.2
GCC_USE_PPL_CLOOG=yes
PPL_VERSION=0.11.2
CLOOG_VERSION=0.15.10
GCC_USE_LTO=yes

# If $(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS) equals
# $(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS) we can reuse the installed
# buildtools for binutils for building gcc.
AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS := $(AUTOCONF_FOR_BINUTILS_VERSION)-$(AUTOMAKE_FOR_BINUTILS_VERSION)-$(LIBTOOL_FOR_BINUTILS_VERSION)
AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS := $(AUTOCONF_FOR_GCC_VERSION)-$(AUTOMAKE_FOR_GCC_VERSION)-$(LIBTOOL_FOR_GCC_VERSION)

# Notes:
#   1) Configure options are explained at http://gcc.gnu.org/install/configure.html
#      but note that this explanation is for the latest gcc version released and
#      not necessary the gcc version we're building here.
ifeq ($(TARGET),arm-unknown-riscos)
# Case GCCSDK arm-unknown-riscos target:
# Specifying --disable-multilib will significantly reduce build times but then
# you only end up with UnixLib/softfloat support.
# Note: --enable-multilib is the default.  Don't specify it unless you want to get hit
# by PR43328/PR45174.
## FIXME: Consider --enable-__cxa_atexit (but this can require UnixLib changes).
GCC_CONFIG_ARGS := \
	--enable-threads=posix \
	--enable-sjlj-exceptions=no \
	--enable-c99 \
	--enable-cmath \
	--disable-c-mbchar \
	--disable-wchar_t \
	--disable-libstdcxx-pch \
	--disable-tls
# FIXME: for Java support: --without-x --enable-libgcj
BINUTILS_CONFIG_ARGS :=
GDB_CONFIG_ARGS :=
else
# Case arm-unknown-eabi target (use newlib):
GCC_CONFIG_ARGS := --disable-threads --disable-multilib target_configargs=--disable-shared --with-newlib
BINUTILS_CONFIG_ARGS := --disable-multilib
GDB_CONFIG_ARGS :=
endif
GCC_CONFIG_ARGS += --with-pkgversion='GCCSDK GCC $(GCC_VERSION) Release 1 Development' \
	--with-bugurl=http://gccsdk.riscos.info/
BINUTILS_CONFIG_ARGS += --with-pkgversion='GCCSDK GCC $(GCC_VERSION) Release 1 Development' \
	--with-bugurl=http://gccsdk.riscos.info/
GDB_CONFIG_ARGS += --with-pkgversion='GCCSDK GCC $(GCC_VERSION) Release 1 Development' \
	--with-bugurl=http://gccsdk.riscos.info/
BINUTILS_CONFIG_ARGS += --enable-maintainer-mode --disable-werror --with-gcc --enable-interwork --disable-nls
# --disable-werror is added because --enable-maintainer-mode turns all warnings into errors and
# the gcc build is not 100% warning free.
# However, this does not help for libstdc++ builds when newlib is used, cfr http://gcc.gnu.org/PR47058.
ifeq ($(TARGET),arm-unknown-riscos)
GCC_CONFIG_ARGS += --enable-maintainer-mode --disable-werror
endif
GCC_CONFIG_ARGS += --enable-interwork --disable-nls --disable-libquadmath
GDB_CONFIG_ARGS += --disable-werror --enable-interwork --disable-multilib --disable-nls

# When debugging/testing/validating the compiler add "--enable-checking=all",
# otherwise add "--enable-checking=release" or even "--enable-checking=no"
GCC_CONFIG_ARGS += --enable-checking=release
# Configure args shared between different targets:
# For debugging:
# FIXME: add to GCC_BUILD_FLAGS for optimized ARM libraries: CFLAGS_FOR_TARGET="-O3 -march=armv5" CXXFLAGS_FOR_TARGET="-O3 -march=armv5"
# Or perhaps better, at GCC configure time something like --with-arch=armv6 --with-tune=cortex-a8 --with-float=softfp --with-fpu=vfp ?
##GCC_BUILD_FLAGS := CFLAGS="-O0 -g" LIBCXXFLAGS="-O0 -g"
##BINUTILS_BUILD_FLAGS = CFLAGS="-O0 -g"

ROOT := $(CURDIR)
PREFIX_CROSS := $(GCCSDK_CROSS_PREFIX)
PREFIX_RONATIVE := $(GCCSDK_RISCOS_PREFIX)/\!GCC
BUILDDIR := $(GCCSDK_BUILDDIR)
BUILDSTEPSDIR := buildstepsdir
# PREFIX_BUILDTOOL_BINUTILS will contain the built autoconf/automake/libtool versions used during building binutils.
PREFIX_BUILDTOOL_BINUTILS := $(BUILDDIR)/installed-buildtools-for-binutils
# PREFIX_BUILDTOOL_GCC will contain the built autoconf/automake/libtool versions used during building gcc.
# When the autoconf/automake/libtool versions needed to build gcc are the same as the autoconf/automake/libtool versions
# needed to build binutils, PREFIX_BUILDTOOL_GCC will be a symlink to PREFIX_BUILDTOOL_BINUTILS
# (compare $(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS) with $(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS)).
PREFIX_BUILDTOOL_GCC := $(BUILDDIR)/installed-buildtools-for-gcc
PREFIX_CROSSGCC_LIBS := $(BUILDDIR)/installed-libs-for-cross-gcc
PREFIX_RONATIVEGCC_LIBS := $(BUILDDIR)/installed-libs-for-ronative-gcc
SRCDIR := $(GCCSDK_SRCDIR)
SRCORIGDIR := $(GCCSDK_SRCDIR).orig
SCRIPTSDIR := $(ROOT)/scripts
RECIPEDIR := $(ROOT)/recipe
RISCOSTOOLSDIR := $(GCCSDK_RISCOS)

# GCC-only configure arguments which are dependant on cross vs ronative building:
CROSS_GCC_CONFIG_ARGS := --with-gmp=$(PREFIX_CROSSGCC_LIBS) --with-mpfr=$(PREFIX_CROSSGCC_LIBS) --with-mpc=$(PREFIX_CROSSGCC_LIBS)
RONATIVE_GCC_CONFIG_ARGS := --with-gmp=$(PREFIX_RONATIVEGCC_LIBS) --with-mpfr=$(PREFIX_RONATIVEGCC_LIBS) --with-mpc=$(PREFIX_RONATIVEGCC_LIBS)
ifeq "$(GCC_USE_PPL_CLOOG)" "yes"
CROSS_GCC_CONFIG_ARGS += --with-ppl=$(PREFIX_CROSSGCC_LIBS) --with-host-libstdcxx='-lstdc++' --with-cloog=$(PREFIX_CROSSGCC_LIBS)
RONATIVE_GCC_CONFIG_ARGS += --with-ppl=$(PREFIX_RONATIVEGCC_LIBS) --with-host-libstdcxx='-lstdc++' --with-cloog=$(PREFIX_RONATVEGCC_LIBS)
endif
ifeq ($(CROSS_ENABLE_SHARED),yes)
CROSS_GCC_CONFIG_ARGS += target_configargs=--enable-shared=libunixlib,libgcc,libstdc++
else
CROSS_GCC_CONFIG_ARGS += target_configargs=--disable-shared
endif
ifeq ($(RONATIVE_ENABLE_SHARED),yes)
RONATIVE_GCC_CONFIG_ARGS += target_configargs=--enable-shared=libunixlib,libgcc,libstdc++
else
# If we're not going to have shared target libraries on RISC OS, we can't build/run our
# native compiler with shared libs either.
RONATIVE_GCC_CONFIG_ARGS += --disable-shared
endif
ifeq ($(GCC_USE_LTO),yes)
CROSS_GCC_CONFIG_ARGS += --enable-lto
else
CROSS_GCC_CONFIG_ARGS += --disable-lto
endif
# If you want LTO support, you need shared library support (as it is a plugin for gcc):
ifeq ($(GCC_USE_LTO)$(RONATIVE_ENABLE_SHARED),yesyes)
RONATIVE_GCC_CONFIG_ARGS += --enable-lto
else
RONATIVE_GCC_CONFIG_ARGS += --disable-lto
endif

# Configure arguments Binutils & GCC:
CROSS_CONFIG_ARGS := --target=$(TARGET) --prefix=$(PREFIX_CROSS)
RONATIVE_CONFIG_ARGS := --host=$(TARGET) --target=$(TARGET) --prefix=$(PREFIX_RONATIVE)

# Configure arguments GMP:
CROSS_GMP_CONFIG_ARGS := --disable-shared --prefix=$(PREFIX_CROSSGCC_LIBS)
RONATIVE_GMP_CONFIG_ARGS := --disable-shared --host=$(TARGET) --prefix=$(PREFIX_RONATIVEGCC_LIBS)
ifeq "$(GCC_USE_PPL_CLOOG)" "yes"
CROSS_GMP_CONFIG_ARGS += --enable-cxx
RONATIVE_GMP_CONFIG_ARGS += --enable-cxx
endif

# Configure arguments MPC:
CROSS_MPC_CONFIG_ARGS := --disable-shared --with-gmp=$(PREFIX_CROSSGCC_LIBS) --with-mpfr=$(PREFIX_CROSSGCC_LIBS) --prefix=$(PREFIX_CROSSGCC_LIBS)
RONATIVE_MPC_CONFIG_ARGS := --disable-shared --with-gmp=$(PREFIX_RONATIVEGCC_LIBS) --with-mpfr=$(PREFIX_RONATIVEGCC_LIBS) --host=$(TARGET) --prefix=$(PREFIX_RONATIVEGCC_LIBS)

# Configure arguments MPFR:
CROSS_MPFR_CONFIG_ARGS := --disable-shared --with-gmp=$(PREFIX_CROSSGCC_LIBS) --prefix=$(PREFIX_CROSSGCC_LIBS)
RONATIVE_MPFR_CONFIG_ARGS := --disable-shared --with-gmp=$(PREFIX_RONATIVEGCC_LIBS) --host=$(TARGET) --prefix=$(PREFIX_RONATIVEGCC_LIBS)

# Configure arguments PPL:
CROSS_PPL_CONFIG_ARGS := --disable-shared --disable-watchdog --with-gnu-ld --with-gmp-prefix=$(PREFIX_CROSSGCC_LIBS) --prefix=$(PREFIX_CROSSGCC_LIBS)
RONATIVE_PPL_CONFIG_ARGS := --disable-shared --disable-watchdog --with-gnu-ld --with-gmp-prefix=$(PREFIX_RONATIVEGCC_LIBS) --host=$(TARGET) --prefix=$(PREFIX_RONATIVEGCC_LIBS)

# Configure arguments CLooG:
CROSS_CLOOG_CONFIG_ARGS := --disable-shared --with-gmp=$(PREFIX_CROSSGCC_LIBS) --with-bits=gmp --with-ppl=$(PREFIX_CROSSGCC_LIBS) --with-host-libstdcxx='-lstdc++' --prefix=$(PREFIX_CROSSGCC_LIBS)
RONATIVE_CLOOG_CONFIG_ARGS := --disable-shared --with-gmp=$(PREFIX_RONATIVEGCC_LIBS) --with-bits=gmp --with-ppl=$(PREFIX_RONATIVEGCC_LIBS) --with-host-libstdcxx='-lstdc++' --host=$(TARGET) --prefix=$(PREFIX_RONATIVEGCC_LIBS)

# To respawn Makefile with setup-gccsdk-param environment loaded.
GCCSDK_INTERNAL_GETENV=getenv
ifeq ($(MAKECMDGOALS),)
MAKECMDGOALS=all
endif

.NOTPARALLEL:
.PHONY: all all-done cross ronative cross-gdb clean clean-done clean-cross clean-cross-done clean-ronative clean-ronative-done distclean distclean-done updategcc updategcc-done getenv
VPATH = $(BUILDSTEPSDIR)

# Default target is to build the cross-compiler (including the RISC OS tools):
all: $(GCCSDK_INTERNAL_GETENV)
all-done: cross-done

# Builds the cross compiler:
cross: $(GCCSDK_INTERNAL_GETENV)
cross-done: cross-gcc-built cross-riscostools-built
	touch $(BUILDSTEPSDIR)/$@

# Builds the native RISC OS compiler (gcc and binutils):
ronative: $(GCCSDK_INTERNAL_GETENV)
ronative-done: ronative-gcc-built ronative-binutils-built ronative-riscostools-built
	touch $(BUILDSTEPSDIR)/$@

# Cross gdb:
cross-gdb: $(GCCSDK_INTERNAL_GETENV)
cross-gdb-done: cross-gdb-built
	touch $(BUILDSTEPSDIR)/$@

clean: $(GCCSDK_INTERNAL_GETENV)
clean-done:
	-rm -rf $(BUILDDIR) $(BUILDSTEPSDIR) $(SRCDIR) $(GCCSDK_CROSS_PREFIX) $(GCCSDK_RISCOS_PREFIX)
	-svn revert -R $(SRCORIGDIR)/gcc-trunk
	-svn status $(SRCORIGDIR)/gcc-trunk | grep -E "^?" | cut -b 9- | xargs rm -rf
	for i in . riscos/asasm riscos/asasm/decaof/ riscos/asasm/elftoolchain/ ; do svn status --no-ignore $$i | grep "^I       " | cut -b 9- | grep -v -E "^(gccsdk-params|srcdir\.orig|release-area)$$" | xargs rm -rf ; done

# Return to a state for doing a full fresh cross build (using the current binutils/gcc sources).
clean-cross: $(GCCSDK_INTERNAL_GETENV)
clean-cross-done:
	-rm -rf $(BUILDSTEPSDIR)/cross-* $(BUILDDIR)/cross* $(GCCSDK_CROSS_PREFIX)

# Return to a state for doing a full fresh ronative build (using the current binutils/gcc sources).
clean-ronative: $(GCCSDK_INTERNAL_GETENV)
clean-ronative-done:
	-rm -rf $(BUILDSTEPSDIR)/ronative-* $(BUILDDIR)/ronative* $(GCCSDK_RISCOS_PREFIX)

distclean: $(GCCSDK_INTERNAL_GETENV)
distclean-done: clean-done
	-rm -rf release-area $(SRCORIGDIR)

updategcc: $(GCCSDK_INTERNAL_GETENV)
updategcc-done:
ifeq "$(GCC_USE_SCM)" "yes"
	-svn revert -R $(SRCORIGDIR)/gcc-trunk
	-svn status $(SRCORIGDIR)/gcc-trunk | grep -E "^?" | cut -b 9- | xargs rm -rf
	cd $(SRCORIGDIR)/gcc-trunk && ./contrib/gcc_update
else
	@echo "Makefile target updategcc is only supported when GCC sources are fetched from the GCC source control system."
endif

# Respawn Makefile again after having loaded all our GCCSDK environment variables.
ifeq ($(GCCSDK_INTERNAL_GETENV),getenv)
getenv:
	test -d $(BUILDSTEPSDIR) || mkdir $(BUILDSTEPSDIR)
	@bash -c ". ./setup-gccsdk-params && $(MAKE) $(patsubst %,%-done,$(MAKECMDGOALS)) GCCSDK_INTERNAL_GETENV="
endif

# -- Configure & building:

# Configure & build autoconf-for-binutils tool:
buildtool-autoconf-for-binutils-built: src-autoconf-for-binutils-copied
	-rm -rf $(BUILDDIR)/buildtool-autoconf-for-binutils
	mkdir -p $(BUILDDIR)/buildtool-autoconf-for-binutils
	cd $(BUILDDIR)/buildtool-autoconf-for-binutils && $(SRCDIR)/autoconf-for-binutils/configure --prefix=$(PREFIX_BUILDTOOL_BINUTILS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

ifneq ($(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS),$(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS))
# Configure & build autoconf-for-gcc tool:
buildtool-autoconf-for-gcc-built: src-autoconf-for-gcc-copied
	-rm -rf $(BUILDDIR)/buildtool-autoconf-for-gcc
	mkdir -p $(BUILDDIR)/buildtool-autoconf-for-gcc
	cd $(BUILDDIR)/buildtool-autoconf-for-gcc && $(SRCDIR)/autoconf-for-gcc/configure --prefix=$(PREFIX_BUILDTOOL_GCC) && $(MAKE) && $(MAKE) install
else
# autoconf-for-gcc, automake-for-gcc and libtool-for-gcc are the same as autoconf-for-binutils, automake-for-binutils and libtool-for-binutils so we can use PREFIX_BUILDTOOL_GCC as PREFIX_BUILDTOOL_BINUTILS
buildtool-autoconf-for-gcc-built: buildtool-autoconf-for-binutils-built
	ln -f -s $(PREFIX_BUILDTOOL_BINUTILS) $(PREFIX_BUILDTOOL_GCC)
endif
	touch $(BUILDSTEPSDIR)/$@

# Configure & build automake-for-binutils tool:
buildtool-automake-for-binutils-built: src-automake-for-binutils-copied buildtool-autoconf-for-binutils-built
	-rm -rf $(BUILDDIR)/buildtool-automake-for-binutils
	mkdir -p $(BUILDDIR)/buildtool-automake-for-binutils
	cd $(BUILDDIR)/buildtool-automake-for-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SRCDIR)/automake-for-binutils/configure --prefix=$(PREFIX_BUILDTOOL_BINUTILS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

ifneq ($(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS),$(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS))
# Configure & build automake-for-gcc tool:
buildtool-automake-for-gcc-built: src-automake-for-gcc-copied buildtool-autoconf-for-gcc-built
	-rm -rf $(BUILDDIR)/buildtool-automake-for-gcc
	mkdir -p $(BUILDDIR)/buildtool-automake-for-gcc
	cd $(BUILDDIR)/buildtool-automake-for-gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SRCDIR)/automake-for-gcc/configure --prefix=$(PREFIX_BUILDTOOL_GCC) && $(MAKE) && $(MAKE) install
else
# autoconf-for-gcc, automake-for-gcc and libtool-for-gcc are the same as autoconf-for-binutils, automake-for-binutils and libtool-for-binutils so we can use PREFIX_BUILDTOOL_GCC as PREFIX_BUILDTOOL_BINUTILS
buildtool-automake-for-gcc-built: buildtool-automake-for-binutils-built
endif
	touch $(BUILDSTEPSDIR)/$@

# Configure & build libtool-for-binutils tool:
buildtool-libtool-for-binutils-built: src-libtool-for-binutils-copied buildtool-autoconf-for-binutils-built buildtool-automake-for-binutils-built
	-rm -rf $(BUILDDIR)/buildtool-libtool-for-binutils
	mkdir -p $(BUILDDIR)/buildtool-libtool-for-binutils
	cd $(BUILDDIR)/buildtool-libtool-for-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SRCDIR)/libtool-for-binutils/configure --prefix=$(PREFIX_BUILDTOOL_BINUTILS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

ifneq ($(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS),$(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS))
# Configure & build libtool-for-gcc tool:
buildtool-libtool-for-gcc-built: src-libtool-for-gcc-copied buildtool-autoconf-for-gcc-built buildtool-automake-for-gcc-built
	-rm -rf $(BUILDDIR)/buildtool-libtool-for-gcc
	mkdir -p $(BUILDDIR)/buildtool-libtool-for-gcc
	cd $(BUILDDIR)/buildtool-libtool-for-gcc && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SRCDIR)/libtool-for-gcc/configure --prefix=$(PREFIX_BUILDTOOL_GCC) && $(MAKE) && $(MAKE) install
else
# autoconf-for-gcc, automake-for-gcc and libtool-for-gcc are the same as autoconf-for-binutils, automake-for-binutils and libtool-for-binutils so we can use PREFIX_BUILDTOOL_GCC as PREFIX_BUILDTOOL_BINUTILS
buildtool-libtool-for-gcc-built: buildtool-libtool-for-binutils-built
endif
	touch $(BUILDSTEPSDIR)/$@

# Configure binutils cross:
cross-binutils-configured: src-binutils-copied
	-rm -rf $(BUILDDIR)/cross-binutils
	mkdir -p $(BUILDDIR)/cross-binutils
	cd $(BUILDDIR)/cross-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SRCDIR)/binutils/configure $(CROSS_CONFIG_ARGS) $(BINUTILS_CONFIG_ARGS)
	touch $(BUILDSTEPSDIR)/$@

# Build binutils cross:
cross-binutils-built: cross-binutils-configured
	cd $(BUILDDIR)/cross-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SRCDIR)/binutils/configure $(CROSS_CONFIG_ARGS) $(BINUTILS_CONFIG_ARGS) && $(MAKE) $(BINUTILS_BUILD_FLAGS) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure ronative:
ronative-binutils-configured: src-binutils-copied cross-done
	-rm -rf $(BUILDDIR)/ronative-binutils
	mkdir -p $(BUILDDIR)/ronative-binutils
	cd $(BUILDDIR)/ronative-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/binutils/configure $(RONATIVE_CONFIG_ARGS) $(BINUTILS_CONFIG_ARGS)
	touch $(BUILDSTEPSDIR)/$@

# Build binutils ronative:
ronative-binutils-built: ronative-binutils-configured
	cd $(BUILDDIR)/ronative-binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/binutils/configure $(RONATIVE_CONFIG_ARGS) $(BINUTILS_CONFIG_ARGS) && $(MAKE) $(BINUTILS_BUILD_FLAGS) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure gcc cross:
ifneq ($(TARGET),arm-unknown-riscos)
cross-gcc-configured: src-newlib-copied
endif
ifeq "$(GCC_USE_PPL_CLOOG)" "yes"
cross-gcc-configured: cross-ppl-built cross-cloog-built
endif
cross-gcc-configured: src-gcc-copied cross-binutils-built cross-gmp-built cross-mpc-built cross-mpfr-built
	-rm -rf $(SRCDIR)/gcc/newlib
	-rm -rf $(SRCDIR)/gcc/libgloss
ifneq ($(TARGET),arm-unknown-riscos)
	ln -s $(SRCDIR)/newlib/newlib $(SRCDIR)/gcc/newlib
	ln -s $(SRCDIR)/newlib/libgloss $(SRCDIR)/gcc/libgloss
	touch $(BUILDSTEPSDIR)/src-gcc-copied
endif
	-rm -rf $(BUILDDIR)/cross-gcc
	mkdir -p $(BUILDDIR)/cross-gcc
	cd $(BUILDDIR)/cross-gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gcc/configure $(CROSS_CONFIG_ARGS) $(CROSS_GCC_CONFIG_ARGS) $(GCC_CONFIG_ARGS) --enable-languages=$(GCC_LANGUAGES)
	touch $(BUILDSTEPSDIR)/$@

# Build gcc cross:
cross-gcc-built: cross-gcc-configured
	cd $(BUILDDIR)/cross-gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(MAKE) $(GCC_BUILD_FLAGS) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure gcc ronative:
ifeq "$(GCC_USE_PPL_CLOOG)" "yes"
ronative-gcc-configured: ronative-ppl-built ronative-cloog-built
endif
ronative-gcc-configured: cross-done ronative-gmp-built ronative-mpc-built ronative-mpfr-built
	-rm -rf $(BUILDDIR)/ronative-gcc
	mkdir -p $(BUILDDIR)/ronative-gcc
	cd $(BUILDDIR)/ronative-gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gcc/configure $(RONATIVE_CONFIG_ARGS) $(RONATIVE_GCC_CONFIG_ARGS) $(GCC_CONFIG_ARGS) --enable-languages=$(GCC_LANGUAGES)
	touch $(BUILDSTEPSDIR)/$@

# Build gcc ronative:
ronative-gcc-built: ronative-gcc-configured
	cd $(BUILDDIR)/ronative-gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(MAKE) $(GCC_BUILD_FLAGS) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build gmp cross:
cross-gmp-built: src-gmp-copied
	-rm -rf $(BUILDDIR)/cross-gmp
	mkdir -p $(BUILDDIR)/cross-gmp
	cd $(BUILDDIR)/cross-gmp && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SRCDIR)/gmp/configure $(CROSS_GMP_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build gmp ronative:
ronative-gmp-built: cross-gcc-built src-gmp-copied
	-rm -rf $(BUILDDIR)/ronative-gmp
	mkdir -p $(BUILDDIR)/ronative-gmp
	cd $(BUILDDIR)/ronative-gmp && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gmp/configure $(RONATIVE_GMP_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build mpc cross:
cross-mpc-built: src-mpc-copied cross-gmp-built cross-mpfr-built
	-rm -rf $(BUILDDIR)/cross-mpc
	mkdir -p $(BUILDDIR)/cross-mpc
	cd $(BUILDDIR)/cross-mpc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SRCDIR)/mpc/configure $(CROSS_MPC_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build mpc ronative:
ronative-mpc-built: cross-gcc-built src-mpc-copied ronative-gmp-built ronative-mpfr-built
	-rm -rf $(BUILDDIR)/ronative-mpc
	mkdir -p $(BUILDDIR)/ronative-mpc
	cd $(BUILDDIR)/ronative-mpc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/mpc/configure $(RONATIVE_MPC_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build mpfr cross:
cross-mpfr-built: src-mpfr-copied cross-gmp-built
	-rm -rf $(BUILDDIR)/cross-mpfr
	mkdir -p $(BUILDDIR)/cross-mpfr
	cd $(BUILDDIR)/cross-mpfr && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SRCDIR)/mpfr/configure $(CROSS_MPFR_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build mpfr ronative:
ronative-mpfr-built: cross-gcc-built src-mpfr-copied ronative-gmp-built
	-rm -rf $(BUILDDIR)/ronative-mpfr
	mkdir -p $(BUILDDIR)/ronative-mpfr
	cd $(BUILDDIR)/ronative-mpfr && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/mpfr/configure $(RONATIVE_MPFR_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build ppl cross:
cross-ppl-built: src-ppl-copied cross-gmp-built
	-rm -rf $(BUILDDIR)/cross-ppl
	mkdir -p $(BUILDDIR)/cross-ppl
	cd $(BUILDDIR)/cross-ppl && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SRCDIR)/ppl/configure $(CROSS_PPL_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build ppl ronative:
ronative-ppl-built: cross-gcc-built src-ppl-copied ronative-gmp-built
	-rm -rf $(BUILDDIR)/ronative-ppl
	mkdir -p $(BUILDDIR)/ronative-ppl
	cd $(BUILDDIR)/ronative-ppl && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/ppl/configure $(RONATIVE_PPL_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build cloog cross:
cross-cloog-built: src-cloog-copied cross-gmp-built cross-ppl-built
	-rm -rf $(BUILDDIR)/cross-cloog
	mkdir -p $(BUILDDIR)/cross-cloog
	cd $(BUILDDIR)/cross-cloog && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SRCDIR)/cloog/configure $(CROSS_CLOOG_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build cloog ronative:
ronative-cloog-built: cross-gcc-built src-cloog-copied ronative-gmp-built ronative-ppl-built
	-rm -rf $(BUILDDIR)/ronative-cloog
	mkdir -p $(BUILDDIR)/ronative-cloog
	cd $(BUILDDIR)/ronative-cloog && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/cloog/configure $(RONATIVE_CLOOG_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Configure & build gdb cross:
cross-gdb-built: src-gdb-copied
	-rm -rf $(BUILDDIR)/cross-gdb
	mkdir -p $(BUILDDIR)/cross-gdb
	cd $(BUILDDIR)/cross-gdb && PATH="$(PREFIX_CROSS)/bin:$(PATH)" && $(SRCDIR)/gdb/configure $(CROSS_CONFIG_ARGS) $(GDB_CONFIG_ARGS) && $(MAKE) && $(MAKE) install
	touch $(BUILDSTEPSDIR)/$@

# Build the RISC OS related tools (cmunge, elf2aif, asasm, etc) cross:
cross-riscostools-built: cross-gcc-built
	cd $(RISCOSTOOLSDIR) && ./build-it cross
	touch $(BUILDSTEPSDIR)/$@

# Build the RISC OS related tools (cmunge, elf2aif, asasm, etc) ronative:
ronative-riscostools-built: ronative-gcc-built
	cd $(RISCOSTOOLSDIR) && ./build-it riscos
	touch $(BUILDSTEPSDIR)/$@

# -- Source unpacking.

# Unpack & copy autoconf-for-binutils source:
src-autoconf-for-binutils-copied: $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION) $(SRCDIR)/autoconf-for-binutils
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/autoconf-for-binutils
	cp -T -p -r $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION) $(SRCDIR)/autoconf-for-binutils
	touch $(BUILDSTEPSDIR)/$@

ifneq ($(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS),$(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS))
# Unpack & copy autoconf-for-gcc source:
src-autoconf-for-gcc-copied: $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION) $(SRCDIR)/autoconf-for-gcc
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/autoconf-for-gcc
	cp -T -p -r $(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION) $(SRCDIR)/autoconf-for-gcc
	touch $(BUILDSTEPSDIR)/$@
endif

# Unpack & copy automake-for-binutils source:
src-automake-for-binutils-copied: $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION) $(SRCDIR)/automake-for-binutils
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/automake-for-binutils
	cp -T -p -r $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION) $(SRCDIR)/automake-for-binutils
	touch $(BUILDSTEPSDIR)/$@

ifneq ($(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS),$(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS))
# Unpack & copy automake-for-gcc source:
src-automake-for-gcc-copied: $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION) $(SRCDIR)/automake-for-gcc
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/automake-for-gcc
	cp -T -p -r $(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION) $(SRCDIR)/automake-for-gcc
	touch $(BUILDSTEPSDIR)/$@
endif

# Unpack & copy libtool-for-binutils source:
src-libtool-for-binutils-copied: $(SRCORIGDIR)/libtool-$(LIBTOOL_FOR_BINUTILS_VERSION).tar.gz
	-rm -rf $(SRCORIGDIR)/libtool-$(LIBTOOL_FOR_BINUTILS_VERSION) $(SRCDIR)/libtool-for-binutils
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/libtool-for-binutils
	cp -T -p -r $(SRCORIGDIR)/libtool-$(LIBTOOL_FOR_BINUTILS_VERSION) $(SRCDIR)/libtool-for-binutils
	touch $(BUILDSTEPSDIR)/$@

ifneq ($(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_BINUTILS_VERSIONS),$(AUTOCONF_AUTOMAKE_LIBTOOL_FOR_GCC_VERSIONS))
# Unpack & copy libtool-for-gcc source:
src-libtool-for-gcc-copied: $(SRCORIGDIR)/libtool-$(LIBTOOL_FOR_GCC_VERSION).tar.gz
	-rm -rf $(SRCORIGDIR)/libtool-$(LIBTOOL_FOR_GCC_VERSION) $(SRCDIR)/libtool-for-gcc
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/libtool-for-gcc
	cp -T -p -r $(SRCORIGDIR)/libtool-$(LIBTOOL_FOR_GCC_VERSION) $(SRCDIR)/libtool-for-gcc
	touch $(BUILDSTEPSDIR)/$@
endif

# Unpack & copy (and optionally patch) binutils source:
# As we're patching and (re)configuring, we need built & installed versions of
# autoconf, automake and libtool.
src-binutils-copied: $(SRCORIGDIR)/binutils-$(BINUTILS_VERSION).tar.bz2 buildtool-autoconf-for-binutils-built buildtool-automake-for-binutils-built buildtool-libtool-for-binutils-built
	-rm -rf $(SRCORIGDIR)/binutils-$(BINUTILS_VERSION) $(SRCDIR)/binutils
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/binutils
	cp -T -p -r $(SRCORIGDIR)/binutils-$(BINUTILS_VERSION) $(SRCDIR)/binutils
ifeq ($(TARGET),arm-unknown-riscos)
	cd $(SRCDIR)/binutils && PATH="$(PREFIX_BUILDTOOL_BINUTILS)/bin:$(PATH)" && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
endif
	touch $(BUILDSTEPSDIR)/$@

# Unpack & copy or checkout the gcc source:
# As we're patching and (re)configuring, we need built & installed versions of
# autoconf, automake and libtool.
src-gcc-copied: buildtool-autoconf-for-gcc-built buildtool-automake-for-gcc-built buildtool-libtool-for-gcc-built
ifeq "$(GCC_USE_SCM)" "yes"
src-gcc-copied: $(SRCORIGDIR)/gcc-trunk/LAST_UPDATED
	-rm -rf $(SRCDIR)/gcc
	-svn revert -R $(SRCORIGDIR)/gcc-trunk
	-svn status $(SRCORIGDIR)/gcc-trunk | grep -E "^?" | cut -b 9- | xargs rm -rf
	ln -s $(SRCORIGDIR)/gcc-trunk $(SRCDIR)/gcc
else
src-gcc-copied: $(SRCORIGDIR)/gcc-$(GCC_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/gcc-$(GCC_VERSION) $(SRCDIR)/gcc
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/gcc
	cp -T -p -r $(SRCORIGDIR)/gcc-$(GCC_VERSION) $(SRCDIR)/gcc
endif
ifeq ($(TARGET),arm-unknown-riscos)
	cd $(SRCDIR)/gcc && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	echo "/* Automatically generated from the value of GCCSDK_RISCOS_ABI_VERSION" > $(SRCDIR)/gcc/gcc/config/arm/riscos-abi.h
	echo "   defined in the file setup_gccsdk_params.  */" >> $(SRCDIR)/gcc/gcc/config/arm/riscos-abi.h
	echo \"$(GCCSDK_RISCOS_ABI_VERSION)\" >> $(SRCDIR)/gcc/gcc/config/arm/riscos-abi.h
endif
	touch $(BUILDSTEPSDIR)/$@

# Unpack & copy gmp source:
src-gmp-copied: $(SRCORIGDIR)/gmp-$(GMP_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/gmp-$(GMP_VERSION) $(SRCDIR)/gmp
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/gmp
	cp -T -p -r $(SRCORIGDIR)/gmp-$(GMP_VERSION) $(SRCDIR)/gmp
ifeq ($(TARGET),arm-unknown-riscos)
	cd $(SRCDIR)/gmp && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
endif
	touch $(BUILDSTEPSDIR)/$@

# Unpack & copy mpc source:
src-mpc-copied: $(SRCORIGDIR)/mpc-$(MPC_VERSION).tar.gz
	-rm -rf $(SRCORIGDIR)/mpc-$(MPC_VERSION) $(SRCDIR)/mpc
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/mpc
	cp -T -p -r $(SRCORIGDIR)/mpc-$(MPC_VERSION) $(SRCDIR)/mpc
	touch $(BUILDSTEPSDIR)/$@

# Unpack & copy mpfr source:
src-mpfr-copied: $(SRCORIGDIR)/mpfr-$(MPFR_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/mpfr-$(MPFR_VERSION) $(SRCDIR)/mpfr
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/mpfr
	cp -T -p -r $(SRCORIGDIR)/mpfr-$(MPFR_VERSION) $(SRCDIR)/mpfr
	touch $(BUILDSTEPSDIR)/$@

# Unpack & copy ppl source:
src-ppl-copied: $(SRCORIGDIR)/ppl-$(PPL_VERSION).tar.gz
	-rm -rf $(SRCORIGDIR)/ppl-$(PPL_VERSION) $(SRCDIR)/ppl
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/ppl
	cp -T -p -r $(SRCORIGDIR)/ppl-$(PPL_VERSION) $(SRCDIR)/ppl
	touch $(BUILDSTEPSDIR)/$@

# Unpack & copy cloog source:
src-cloog-copied: $(SRCORIGDIR)/cloog-ppl-$(CLOOG_VERSION).tar.gz
	-rm -rf $(SRCORIGDIR)/cloog-ppl-$(CLOOG_VERSION) $(SRCDIR)/cloog
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/cloog
	cp -T -p -r $(SRCORIGDIR)/cloog-ppl-$(CLOOG_VERSION) $(SRCDIR)/cloog
	# The following is temporary fix for CLoog 0.15.10
	cd $(SRCDIR)/cloog && PATH="$(PREFIX_BUILDTOOL_GCC)/bin:$(PATH)" && $(SCRIPTSDIR)/do-patch-and-copy $(RECIPEDIR)
	touch $(BUILDSTEPSDIR)/$@

# Unpack & copy or checkout & link newlib source:
ifeq "$(NEWLIB_USE_SCM)" "yes"
src-newlib-copied:
	-rm -rf $(SRCDIR)/newlib
	-mkdir -p $(SRCORIGDIR)/newlib-cvs
	# FIXME? Use -d cvs co option ?
	cd $(SRCORIGDIR)/newlib-cvs && cvs -z 9 -d :pserver:anoncvs@sources.redhat.com:/cvs/src co newlib
	ln -s $(SRCORIGDIR)/newlib-cvs/src $(SRCDIR)/newlib
	touch $(BUILDSTEPSDIR)/$@
else
src-newlib-copied: $(SRCORIGDIR)/newlib-$(NEWLIB_VERSION).tar.gz
	-rm -rf $(SRCORIGDIR)/newlib-$(NEWLIB_VERSION) $(SRCDIR)/newlib
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/newlib
	cp -T -p -r $(SRCORIGDIR)/newlib-$(NEWLIB_VERSION) $(SRCDIR)/newlib
	touch $(BUILDSTEPSDIR)/$@
endif

# Unpack & copy gdb source:
src-gdb-copied: $(SRCORIGDIR)/gdb-$(GDB_VERSION).tar.bz2
	-rm -rf $(SRCORIGDIR)/gdb-$(GDB_VERSION) $(SRCDIR)/gdb
	cd $(SRCORIGDIR) && tar xf $<
	-mkdir -p $(SRCDIR)/gdb
	cp -T -p -r $(SRCORIGDIR)/gdb-$(GDB_VERSION) $(SRCDIR)/gdb
	touch $(BUILDSTEPSDIR)/$@

# -- Source downloading.

# Download autoconf source to be used to build binutils:
$(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/autoconf/autoconf-$(AUTOCONF_FOR_BINUTILS_VERSION).tar.bz2
	touch $@

# Download automake source to be used to build binutils/autoconf-for-binutils:
$(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/automake/automake-$(AUTOMAKE_FOR_BINUTILS_VERSION).tar.bz2
	touch $@

# Download libtool source to be used to build binutils/autoconf-for-binutils:
$(SRCORIGDIR)/libtool-$(LIBTOOL_FOR_BINUTILS_VERSION).tar.gz:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/libtool/libtool-$(LIBTOOL_FOR_BINUTILS_VERSION).tar.gz
	touch $@

ifneq ($(AUTOCONF_FOR_BINUTILS_VERSION),$(AUTOCONF_FOR_GCC_VERSION))
# Download autoconf source to be used to build gcc:
$(SRCORIGDIR)/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/autoconf/autoconf-$(AUTOCONF_FOR_GCC_VERSION).tar.bz2
	touch $@
endif

ifneq ($(AUTOMAKE_FOR_BINUTILS_VERSION),$(AUTOMAKE_FOR_GCC_VERSION))
# Download automake source to be used to build gcc:
$(SRCORIGDIR)/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/automake/automake-$(AUTOMAKE_FOR_GCC_VERSION).tar.bz2
	touch $@
endif

ifneq ($(LIBTOOL_FOR_BINUTILS_VERSION),$(LIBTOOL_FOR_GCC_VERSION))
# Download libtool source to be used to build gcc:
$(SRCORIGDIR)/libtool-$(LIBTOOL_FOR_GCC_VERSION).tar.gz:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/libtool/libtool-$(LIBTOOL_FOR_GCC_VERSION).tar.gz
	touch $@
endif

# Download binutils source:
$(SRCORIGDIR)/binutils-$(BINUTILS_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/binutils/binutils-$(BINUTILS_VERSION).tar.bz2
	touch $@

ifeq "$(GCC_USE_SCM)" "yes"
# Checkout gcc source:
$(SRCORIGDIR)/gcc-trunk/LAST_UPDATED:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && svn co svn://gcc.gnu.org/svn/gcc/$(GCC_SCM_PATH) gcc-trunk
	cd $(SRCORIGDIR)/gcc-trunk && ./contrib/gcc_update
else
# Download gcc source:
$(SRCORIGDIR)/gcc-$(GCC_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.bz2
	touch $@
endif

# Download gmp source:
$(SRCORIGDIR)/gmp-$(GMP_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c ftp://ftp.gmplib.org/pub/gmp-$(GMP_VERSION)/gmp-$(GMP_VERSION).tar.bz2
	touch $@

# Download mpc source:
$(SRCORIGDIR)/mpc-$(MPC_VERSION).tar.gz:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://www.multiprecision.org/mpc/download/mpc-$(MPC_VERSION).tar.gz
	touch $@

# Download mpfr source:
$(SRCORIGDIR)/mpfr-$(MPFR_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://www.mpfr.org/mpfr-$(MPFR_VERSION)/mpfr-$(MPFR_VERSION).tar.bz2
	touch $@

# Download ppl source:
$(SRCORIGDIR)/ppl-$(PPL_VERSION).tar.gz:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://www.cs.unipr.it/ppl/download/ftp/releases/$(PPL_VERSION)/ppl-$(PPL_VERSION).tar.gz
	touch $@

# Download CLooG source:
$(SRCORIGDIR)/cloog-ppl-$(CLOOG_VERSION).tar.gz:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c ftp://gcc.gnu.org/pub/gcc/infrastructure/cloog-ppl-$(CLOOG_VERSION).tar.gz
	touch $@

# Download newlib source:
$(SRCORIGDIR)/newlib-$(NEWLIB_VERSION).tar.gz:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c ftp://sources.redhat.com/pub/newlib/newlib-$(NEWLIB_VERSION).tar.gz
	touch $@

# Download gdb source:
$(SRCORIGDIR)/gdb-$(GDB_VERSION).tar.bz2:
	-mkdir -p $(SRCORIGDIR)
	cd $(SRCORIGDIR) && wget -c http://ftpmirror.gnu.org/gdb/gdb-$(GDB_VERSION).tar.bz2
	touch $@

