# Public parameters: BUILDDIR, INSTALLDIR (= prefix) and CONFIGPARAM (typically
# contain --host).
# Use RONATIVE=yes to build RISC OS native library + test.

ETCROOT := $(CURDIR)/current-trunk
TESTROOT := $(CURDIR)/test

RONATIVE ?= no
ifeq ($(RONATIVE),yes)
BUILDDIR ?= $(CURDIR)/builddir-ro
BUILDDIRTEST ?= $(CURDIR)/builddir-test-ro
INSTALLDIR ?= $(CURDIR)/installdir-ro
CONFIGPARAM ?= --host=arm-unknown-riscos --disable-building-tools
CFLAGS := "-O3 -mlibscl"
CFLAGSTEST := $(CFLAGS)
LDFLAGSTEST := "-L$(INSTALLDIR)/lib -static"
CPPFLAGSTEST := "-I$(INSTALLDIR)/include"
else
BUILDDIR ?= $(CURDIR)/builddir
BUILDDIRTEST ?= $(CURDIR)/builddir-test
INSTALLDIR ?= $(CURDIR)/installdir
CONFIGPARAM ?=
CFLAGS :=
CFLAGSTEST := $(CFLAGS)
LDFLAGSTEST := "-L$(INSTALLDIR)/lib -static"
CPPFLAGSTEST := "-I$(INSTALLDIR)/include"
endif
CONFIGPARAM += --target=arm-unknown-eabi
# CFLAGS += " -O0 -g"

all: etc

install: etc-install

etc: $(BUILDDIR)/etc/GNUmakefile $(ETCROOT)/configure
	make -C $(BUILDDIR)/etc

etc-install: etc $(INSTALLDIR)
	make -C $(BUILDDIR)/etc install

test: $(BUILDDIRTEST)/Makefile
	make -C $(BUILDDIRTEST)

clean:
	make -C $(BUILDDIR)/etc clean

distclean:
	-rm -rf $(BUILDDIR) $(BUILDDIRTEST) $(INSTALLDIR)
	-test -d $(ETCROOT) && cd $(ETCROOT) && rm -rf `svn status --no-ignore | grep "^[I|\?]" | cut -c 9-`
	-test -d $(TESTROOT) && cd $(TESTROOT) && rm -rf `svn status --no-ignore | grep "^[I|\?]" | cut -c 9-`

.PHONY: all install etc etc-install test clean distclean
.NOTPARALLEL:

# --------------------

# Autoconfiguring etc:
$(BUILDDIR)/etc/GNUmakefile: $(ETCROOT)/GNUmakefile.in $(ETCROOT)/configure
	-mkdir -p $(BUILDDIR)/etc
	cd $(BUILDDIR)/etc && sh $(ETCROOT)/configure --prefix=$(INSTALLDIR) $(CONFIGPARAM) CFLAGS=$(CFLAGS)

$(ETCROOT)/GNUmakefile.in: $(ETCROOT)/GNUmakefile.am
	cd $(ETCROOT) && autoreconf --install
 
$(ETCROOT)/configure: $(ETCROOT)/configure.ac
	cd $(ETCROOT) && autoreconf --install

$(BUILDDIRTEST)/Makefile: $(TESTROOT)/Makefile.in $(TESTROOT)/configure
	-mkdir -p $(BUILDDIRTEST)
	cd $(BUILDDIRTEST) && sh $(TESTROOT)/configure --prefix=$(INSTALLDIR) $(CONFIGPARAM) CFLAGS=$(CFLAGSTEST) LDFLAGS=$(LDFLAGSTEST) CPPFLAGS=$(CPPFLAGSTEST)

$(TESTROOT)/Makefile.in: $(TESTROOT)/Makefile.am
	cd $(TESTROOT) && autoreconf --install

$(TESTROOT)/configure: $(TESTROOT)/configure.ac
	cd $(TESTROOT) && autoreconf --install

# --------------------

$(INSTALLDIR):
	mkdir -p $(INSTALLDIR)

