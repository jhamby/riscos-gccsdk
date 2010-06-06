--- Makefile.in.orig	2010-06-05 18:30:35.067247721 +0100
+++ Makefile.in	2010-06-05 18:31:03.677247860 +0100
@@ -20,7 +20,7 @@
 # C Compiler and options for use in building executables that
 # will run on the platform that is doing the build.
 #
-BCC = @BUILD_CC@ @BUILD_CFLAGS@
+BCC = /usr/bin/@BUILD_CC@ @BUILD_CFLAGS@
 
 # C Compile and options for use in building executables that 
 # will run on the target platform.  (BCC and TCC are usually the
