--- coders/Makefile.am.orig	2014-10-04 14:20:31.000000000 +0100
+++ coders/Makefile.am	2015-06-18 17:44:58.693878813 +0100
@@ -172,6 +172,7 @@
 	coders/sfw.c \
 	coders/sgi.c \
 	coders/sixel.c \
+	coders/spr.c \
 	coders/stegano.c \
 	coders/sun.c \
 	coders/svg.c \
@@ -297,6 +298,7 @@
 	coders/sfw.la \
 	coders/sgi.la \
 	coders/sixel.la \
+	coders/spr.la \
 	coders/stegano.la \
 	coders/sun.la \
 	coders/svg.la \
@@ -885,6 +887,12 @@
 coders_scr_la_LDFLAGS      = $(MODULECOMMONFLAGS)
 coders_scr_la_LIBADD       = $(MAGICKCORE_LIBS)
 
+# SPR coder module
+coders_spr_la_SOURCES      = coders/spr.c
+coders_spr_la_CPPFLAGS     = $(MODULE_EXTRA_CPPFLAGS)
+coders_spr_la_LDFLAGS      = $(MODULECOMMONFLAGS)
+coders_spr_la_LIBADD       = $(MAGICKCORE_LIBS)
+
 # SCT coder module
 coders_sct_la_SOURCES      = coders/sct.c
 coders_sct_la_CPPFLAGS     = $(MAGICK_CODER_CPPFLAGS)
