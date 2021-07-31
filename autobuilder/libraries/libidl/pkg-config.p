--- libIDL-config-2.in.orig	2009-07-23 08:27:22.000000000 -0700
+++ libIDL-config-2.in	2009-07-23 08:27:40.000000000 -0700
@@ -72,10 +72,10 @@
 	echo $exec_prefix
 fi
 if test "$echo_cflags" = "yes"; then
-	all_flags="$all_flags $includes $cflags `pkg-config --cflags glib-2.0`"
+	all_flags="$all_flags $includes $cflags `ro-pkg-config --cflags glib-2.0`"
 fi
 if test "$echo_libs" = "yes"; then
-	all_flags="$all_flags -L@libdir@ -lIDL-2 `pkg-config --libs glib-2.0`"
+	all_flags="$all_flags -L@libdir@ -lIDL-2 `ro-pkg-config --libs glib-2.0`"
 fi
 
 # Straight out any possible duplicates, but be careful to
