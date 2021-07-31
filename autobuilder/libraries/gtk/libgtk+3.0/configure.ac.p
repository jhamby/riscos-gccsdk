--- configure.ac.orig	2013-10-17 01:34:10.000000000 +0100
+++ configure.ac	2014-05-11 20:18:40.593274546 +0100
@@ -308,6 +308,10 @@
               [AS_HELP_STRING([--enable-wayland-backend],
                               [enable the wayland gdk backend])],
 			      [backend_set=yes])
+AC_ARG_ENABLE(riscos-backend,
+	      [AS_HELP_STRING([--enable-riscos-backend],
+			      [enable the RISC OS gdk backend])],
+			      [backend_set=yes])
 
 if test -z "$backend_set"; then
   if test "$platform_win32" = yes; then
@@ -407,6 +411,17 @@
 fi
 AC_SUBST(DISABLE_ON_WAYLAND)
 
+if test "x$enable_riscos_backend" == "xyes"; then
+  cairo_backends="$cairo_backends cairo-riscos-sprite"
+  GDK_BACKENDS="$GDK_BACKENDS riscos"
+  have_gio_unix=yes
+  GDK_WINDOWING="$GDK_WINDOWING
+#define GDK_WINDOWING_RISCOS"
+  AM_CONDITIONAL(USE_RISCOS, true)
+else
+  AM_CONDITIONAL(USE_RISCOS, false)
+fi
+
 # strip leading space
 GDK_BACKENDS=${GDK_BACKENDS#* }
 
@@ -1793,6 +1808,7 @@
 gdk/wayland/Makefile
 gdk/tests/Makefile
 gdk/gdkversionmacros.h
+gdk/riscos/Makefile
 gtk/Makefile
 gtk/makefile.msc
 gtk/gtkversion.h
