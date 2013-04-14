--- mono/metadata/Makefile.in.orig	2013-03-08 04:36:23.000000000 +0000
+++ mono/metadata/Makefile.in	2013-04-14 14:47:26.000000000 +0100
@@ -893,10 +893,10 @@
 # The files that use MONO_ASSEMBLIES and/or MONO_CFG_DIR replace the
 # / by \ if running under WIN32.
 @CROSS_COMPILING_TRUE@@HOST_WIN32_TRUE@assembliesdir = ${libdir}
-@HOST_WIN32_FALSE@assembliesdir = $(exec_prefix)/lib
+@HOST_WIN32_FALSE@assembliesdir = /Mono:/lib
 @CROSS_COMPILING_FALSE@@HOST_WIN32_TRUE@confdir = `cygpath -m "${sysconfdir}"`
 @CROSS_COMPILING_TRUE@@HOST_WIN32_TRUE@confdir = ${sysconfdir}
-@HOST_WIN32_FALSE@confdir = $(sysconfdir)
+@HOST_WIN32_FALSE@confdir = /Mono:/etc
 # The mingw math.h has "extern inline" functions that dont appear in libs, so
 # optimisation is required to actually inline them
 @HOST_WIN32_TRUE@AM_CFLAGS = -O
