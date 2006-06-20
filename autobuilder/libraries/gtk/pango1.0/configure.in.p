--- configure.in.org	2006-04-27 06:04:02.000000000 +0200
+++ configure.in	2006-06-16 00:00:16.000000000 +0200
@@ -301,7 +301,7 @@
   pango_save_ldflags=$LDFLAGS
   m4_pattern_allow([PKG_CONFIG_DISABLE_UNINSTALLED])
   INSTALLED_CAIRO_LIBS=`PKG_CONFIG_DISABLE_UNINSTALLED=yes $PKG_CONFIG --libs cairo`
-  LDFLAGS="$LDFLAGS $INSTALLED_CAIRO_LIBS"
+  LDFLAGS="$LDFLAGS $INSTALLED_CAIRO_LIBS $FREETYPE_LIBS -lexpat"

   # A couple temporary checks, to not rely on cairo HEAD :(
   have_cairo_scaled_font_getters=false
