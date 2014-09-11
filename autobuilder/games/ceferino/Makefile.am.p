--- src/Makefile.am.orig	2014-09-11 13:00:55.000000000 +0100
+++ src/Makefile.am	2014-09-11 13:02:17.000000000 +0100
@@ -1,5 +1,5 @@
 INCLUDES = $(DEPS_CFLAGS) -DLOCALEDIR=\""$(datadir)/locale"\" \
-	-DDATADIR=\""$(pkgdatadir)"\"
+	-DDATADIR=\""/<Ceferino\$$Dir>/data"\"
 
 bin_PROGRAMS = ceferino ceferinosetup ceferinoeditor
 
