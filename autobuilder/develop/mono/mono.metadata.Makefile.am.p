--- mono/metadata/Makefile.am.orig	2013-09-11 17:49:01.000000000 +0100
+++ mono/metadata/Makefile.am	2013-09-11 17:49:15.204293487 +0100
@@ -20,8 +20,13 @@
 AM_CFLAGS = -O
 else
 
+if PLATFORM_RISCOS
+assembliesdir = /Mono:/lib
+confdir = /Mono:/etc
+else
 assembliesdir = $(exec_prefix)/lib
 confdir = $(sysconfdir)
+endif
 unix_sources = \
 	console-unix.c
 
