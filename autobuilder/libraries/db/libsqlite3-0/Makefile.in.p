--- Makefile.in.orig	2016-07-17 09:34:21.001958732 +0100
+++ Makefile.in	2016-07-17 09:38:39.101961544 +0100
@@ -20,7 +20,7 @@
 # C Compiler and options for use in building executables that
 # will run on the platform that is doing the build.
 #
-BCC = @BUILD_CC@ @BUILD_CFLAGS@
+BCC = /usr/bin/@BUILD_CC@ @BUILD_CFLAGS@
 
 # TCC is the C Compile and options for use in building executables that 
 # will run on the target platform.  (BCC and TCC are usually the
@@ -97,7 +97,7 @@

 # Filename extensions
 #
-BEXE = @BUILD_EXEEXT@
+BEXE =
 TEXE = @TARGET_EXEEXT@

 # The following variable is "1" if the configure script was able to locate