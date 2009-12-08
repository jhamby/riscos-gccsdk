--- Makefile.in.orig	2009-12-08 09:27:33.000000000 -0800
+++ Makefile.in	2009-12-08 09:34:52.000000000 -0800
@@ -19,7 +19,7 @@
 exec_prefix = $(prefix)
 bindir = $(exec_prefix)/bin
 libdir = $(exec_prefix)/lib
-datadir = $(prefix)/share/xconq
+datadir = /<Xconq$$Dir>/data
 mandir = $(prefix)/man
 man6dir = $(mandir)/man6
 infodir = $(prefix)/info
