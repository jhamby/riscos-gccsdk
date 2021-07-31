--- Makefile.in.orig	2009-12-18 12:24:59.000000000 -0800
+++ Makefile.in	2009-12-18 12:25:14.000000000 -0800
@@ -19,7 +19,7 @@
 exec_prefix = $(prefix)
 bindir = $(exec_prefix)/bin
 libdir = $(exec_prefix)/lib
-datadir = $(prefix)/share/xconq
+datadir = /Xconq:/data
 mandir = $(prefix)/man
 man6dir = $(mandir)/man6
 infodir = $(prefix)/info
@@ -54,7 +54,7 @@
 
 CC = @CC@
 
-CFLAGS = -g
+CFLAGS = -O2
 
 RANLIB = ranlib
 
