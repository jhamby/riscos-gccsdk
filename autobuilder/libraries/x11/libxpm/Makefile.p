--- Makefile.am.orig	2007-11-25 09:44:29.000000000 -0800
+++ Makefile.am	2007-11-25 09:44:43.000000000 -0800
@@ -1,7 +1,7 @@
 # Daniel Stone disowns all copyright on this file; no warranty is given as to its
 # suitability or otherwise.
 
-SUBDIRS = src sxpm cxpm
+SUBDIRS = src cxpm #sxpm
 
 pkgconfigdir = $(libdir)/pkgconfig
 pkgconfig_DATA = xpm.pc
