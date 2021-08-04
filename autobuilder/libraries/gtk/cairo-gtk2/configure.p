--- configure.orig	2021-08-03 22:16:21.575641872 -0700
+++ configure	2021-08-03 22:41:15.943194845 -0700
@@ -22291,7 +22291,7 @@
 		     if test "x$no_x" = xyes; then
 		       use_xlib="no (requires X development libraries)"
 		     else
-		       xlib_NONPKGCONFIG_LIBS="$X_PRE_LIBS $X_LIBS -lX11 -lXext $X_EXTRA_LIBS"
+		       xlib_NONPKGCONFIG_LIBS="$X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"
 		       xlib_NONPKGCONFIG_CFLAGS=$X_CFLAGS
 		     fi
 elif test $pkg_failed = untried; then
