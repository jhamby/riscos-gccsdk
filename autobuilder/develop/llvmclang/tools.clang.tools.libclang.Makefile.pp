--- tools/clang/tools/libclang/Makefile.orig	2014-07-16 17:48:33.000000000 +0100
+++ tools/clang/tools/libclang/Makefile	2015-01-23 18:43:31.122781465 +0000
@@ -33,7 +33,7 @@
 include $(CLANG_LEVEL)/Makefile
 
 # Add soname to the library.
-ifeq ($(HOST_OS), $(filter $(HOST_OS), Linux FreeBSD GNU GNU/kFreeBSD))
+ifeq ($(HOST_OS), $(filter $(HOST_OS), Linux FreeBSD GNU GNU/kFreeBSD RISCOS))
         LLVMLibsOptions += -Wl,-soname,lib$(LIBRARYNAME)$(SHLIBEXT)
 endif
 
