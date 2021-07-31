--- glib/Makefile.in.orig	2013-05-30 01:19:05.528071023 +0100
+++ glib/Makefile.in	2013-05-30 01:19:20.640019148 +0100
@@ -394,7 +394,6 @@
 	$(POPPLER_GLIB_CFLAGS)			\
 	$(FREETYPE_CFLAGS)			\
 	$(FONTCONFIG_CFLAGS)                    \
-	$(POPPLER_GLIB_DISABLE_DEPRECATED)	\
 	$(POPPLER_GLIB_DISABLE_SINGLE_INCLUDES)
 
 stamp_files = stamp-poppler-enums.h
--- glib/demo/Makefile.in.orig	2013-05-30 01:22:54.419291711 +0100
+++ glib/demo/Makefile.in	2013-05-30 01:23:15.443220571 +0100
@@ -316,7 +316,6 @@
 	-I$(top_srcdir)/glib			\
 	-I$(top_builddir)/glib			\
 	$(GTK_TEST_CFLAGS)			\
-	$(POPPLER_GLIB_DISABLE_DEPRECATED)	\
 	-DGTK_DISABLE_DEPRECATED		\
 	$(POPPLER_GLIB_DISABLE_SINGLE_INCLUDES)
 
