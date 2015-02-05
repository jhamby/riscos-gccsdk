--- tools/llvm-shlib/Makefile.orig	2014-05-13 20:37:03.000000000 +0100
+++ tools/llvm-shlib/Makefile	2015-01-23 18:46:43.086786970 +0000
@@ -53,18 +53,18 @@
     LLVMLibsOptions    := $(LLVMLibsOptions) -all_load
 endif
 
-ifeq ($(HOST_OS), $(filter $(HOST_OS), DragonFly Linux FreeBSD GNU/kFreeBSD OpenBSD GNU Bitrig))
+ifeq ($(HOST_OS), $(filter $(HOST_OS), DragonFly Linux FreeBSD GNU/kFreeBSD OpenBSD GNU Bitrig RISCOS))
     # Include everything from the .a's into the shared library.
     LLVMLibsOptions := -Wl,--whole-archive $(LLVMLibsOptions) \
                        -Wl,--no-whole-archive
 endif
 
-ifeq ($(HOST_OS), $(filter $(HOST_OS), DragonFly Linux FreeBSD GNU/kFreeBSD GNU))
+ifeq ($(HOST_OS), $(filter $(HOST_OS), DragonFly Linux FreeBSD GNU/kFreeBSD GNU RISCOS))
     # Add soname to the library.
     LLVMLibsOptions += -Wl,--soname,lib$(LIBRARYNAME)$(SHLIBEXT)
 endif
 
-ifeq ($(HOST_OS), $(filter $(HOST_OS), Linux GNU GNU/kFreeBSD))
+ifeq ($(HOST_OS), $(filter $(HOST_OS), Linux GNU GNU/kFreeBSD RISCOS))
     # Don't allow unresolved symbols.
     LLVMLibsOptions += -Wl,--no-undefined
 endif
