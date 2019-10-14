Index: Source/Makefile.setup.CROSSGCC_ELF
===================================================================
--- Source/Makefile.setup.CROSSGCC_ELF	(revision 455)
+++ Source/Makefile.setup.CROSSGCC_ELF	(working copy)
@@ -16,6 +16,8 @@
 else ifeq ($(ELFOBJECTTYPE),MODULE)
 CCFLAGS = -mmodule
 DEFMODFLAGS = -float-abi hard
+else ifeq ($(ELFOBJECTTYPE),ARMEABIHF)
+# Shoudn't need any special flags as the compiler defaults to the flags we require.
 else
 $(error Unknown ELFOBJECTTYPE value, possible values are SOFTFPU, HARDFPU and MODULE)
 endif
@@ -38,15 +40,25 @@
 export OSLIBSUPPORTDIR := $(BUILDDIR)/OSLibSupport
 
 export BYTEWIDE := $(SOURCEDIR)/ByteWide
+ifeq ($(ELFOBJECTTYPE),ARMEABIHF)
+export DEFMOD := $(BINDIR)/defmod -asmtype gnueabihf $(DEFMODFLAGS)
+else
 export DEFMOD := $(BINDIR)/defmod -asmtype gccelf $(DEFMODFLAGS)
+endif
 export REDUCEAOF := @echo Not needed for ELF build: reduceaof
 export BINDHELP := $(BINDIR)/bindhelp
 
 export PATH := $(GCCSDK_INSTALL_CROSSBIN):$(PATH)
 
+ifeq ($(ELFOBJECTTYPE),ARMEABIHF)
+export CC := arm-riscos-gnueabihf-gcc $(CCFLAGS)
+export CXX := arm-riscos-gnueabihf-g++ $(CCFLAGS)
+export AS := arm-riscos-gnueabihf-gcc -c -xassembler-with-cpp $(CCFLAGS) -I.. -I$(SOURCEDIR)/Types
+else
 export CC := arm-unknown-riscos-gcc $(CCFLAGS)
 export CXX := arm-unknown-riscos-g++ $(CCFLAGS)
 export AS := arm-unknown-riscos-gcc -c -xassembler-with-cpp $(CCFLAGS) -I.. -I$(SOURCEDIR)/Types
+endif
 export LIBFILE := arm-unknown-riscos-ar rs
 export MKDIR := mkdir -p
 export RM := rm -rf
