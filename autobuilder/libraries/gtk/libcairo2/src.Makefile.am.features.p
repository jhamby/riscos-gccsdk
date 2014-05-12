--- src/Makefile.am.features.orig	2013-08-26 18:00:55.000000000 +0100
+++ src/Makefile.am.features	2014-05-11 13:43:31.274377749 +0100
@@ -17,6 +17,38 @@
 all_cairo_pkgconf = cairo.pc
 enabled_cairo_pkgconf = cairo.pc
 
+supported_cairo_headers += $(cairo_riscos_sprite_headers)
+all_cairo_headers += $(cairo_riscos_sprite_headers)
+all_cairo_private += $(cairo_riscos_sprite_private)
+all_cairo_cxx_sources += $(cairo_riscos_sprite_cxx_sources)
+all_cairo_sources += $(cairo_riscos_sprite_sources)
+if CAIRO_HAS_RISCOS_SPRITE_SURFACE
+enabled_cairo_headers += $(cairo_riscos_sprite_headers)
+enabled_cairo_private += $(cairo_riscos_sprite_private)
+enabled_cairo_cxx_sources += $(cairo_riscos_sprite_cxx_sources)
+enabled_cairo_sources += $(cairo_riscos_sprite_sources)
+endif
+all_cairo_pkgconf += cairo-riscos-sprite.pc
+if CAIRO_HAS_RISCOS_SPRITE_SURFACE
+enabled_cairo_pkgconf += cairo-riscos-sprite.pc
+endif
+
+supported_cairo_headers += $(cairo_riscos_font_headers)
+all_cairo_headers += $(cairo_riscos_font_headers)
+all_cairo_private += $(cairo_riscos_font_private)
+all_cairo_cxx_sources += $(cairo_riscos_font_cxx_sources)
+all_cairo_sources += $(cairo_riscos_font_sources)
+if CAIRO_HAS_RISCOS_FONT
+enabled_cairo_headers += $(cairo_riscos_font_headers)
+enabled_cairo_private += $(cairo_riscos_font_private)
+enabled_cairo_cxx_sources += $(cairo_riscos_font_cxx_sources)
+enabled_cairo_sources += $(cairo_riscos_font_sources)
+endif
+all_cairo_pkgconf += cairo-riscos-font.pc
+if CAIRO_HAS_RISCOS_FONT
+enabled_cairo_pkgconf += cairo-riscos-font.pc
+endif
+
 supported_cairo_headers += $(cairo_xlib_headers)
 all_cairo_headers += $(cairo_xlib_headers)
 all_cairo_private += $(cairo_xlib_private)
@@ -655,3 +687,12 @@
 enabled_cairo_cxx_sources += $(cairo_symbol_lookup_cxx_sources)
 enabled_cairo_sources += $(cairo_symbol_lookup_sources)
 endif
+
+all_cairo_private += $(cairo_perf_utils_private) $(cairo_perf_utils_headers)
+all_cairo_cxx_sources += $(cairo_perf_utils_cxx_sources)
+all_cairo_sources += $(cairo_perf_utils_sources)
+if CAIRO_HAS_PERF_UTILS
+enabled_cairo_private += $(cairo_perf_utils_private) $(cairo_perf_utils_headers)
+enabled_cairo_cxx_sources += $(cairo_perf_utils_cxx_sources)
+enabled_cairo_sources += $(cairo_perf_utils_sources)
+endif
