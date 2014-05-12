--- src/Makefile.sources.orig	2013-08-26 16:07:21.000000000 +0100
+++ src/Makefile.sources	2014-05-11 13:28:38.914366185 +0100
@@ -466,3 +466,10 @@
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
