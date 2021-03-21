--- Makefile.am.orig	2021-03-21 17:13:57.770515628 +0000
+++ Makefile.am	2021-03-21 17:14:54.689215562 +0000
@@ -1,3 +1,3 @@
-SUBDIRS=src data Sounds gentoo
+SUBDIRS=src Sounds
 datdir = $(datadir)/@PACKAGE@
 dat_HEADERS = COPYING.Music
