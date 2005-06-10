--- src/client/Makefile.in.orig	2005-06-09 14:19:51.000000000 +0100
+++ src/client/Makefile.in	2005-06-09 14:20:16.000000000 +0100
@@ -19,7 +19,7 @@
 srcdir = @srcdir@
 top_srcdir = @top_srcdir@
 VPATH = @srcdir@
-pkgdatadir = $(datadir)/@PACKAGE@
+pkgdatadir = \<Xpilot-ng\$$dir\>/data
 pkglibdir = $(libdir)/@PACKAGE@
 pkgincludedir = $(includedir)/@PACKAGE@
 top_builddir = ../..
