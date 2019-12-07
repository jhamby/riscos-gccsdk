--- src/Makefile.sources.orig	2016-12-08 01:26:33.000000000 +0000
+++ src/Makefile.sources	2017-04-03 15:35:41.602349757 +0100
@@ -470,3 +470,10 @@
 		     cairo-cogl-gradient.c \
 		     cairo-cogl-context.c \
 		     cairo-cogl-utils.c
+
+cairo_riscos_sprite_headers = cairo-riscos.h
+cairo_riscos_sprite_sources = cairo-riscos-sprite-surface.c \
+			      cairo-riscos-memory.c \
+			      $(NULL)
+
+cairo_riscos_font_sources = cairo-riscos-font.c
