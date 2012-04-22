--- configure.orig	2012-04-22 20:50:14.000000000 +0100
+++ configure	2012-04-22 20:48:57.000000000 +0100
@@ -14545,9 +14545,9 @@
 
 
 
-if test "$enable_static" = "yes"; then
-	as_fn_error $? "p11-kit cannot be used as a static library" "$LINENO" 5
-fi
+#if test "$enable_static" = "yes"; then
+#	as_fn_error $? "p11-kit cannot be used as a static library" "$LINENO" 5
+#fi
 
 { $as_echo "$as_me:${as_lineno-$LINENO}: checking for win32" >&5
 $as_echo_n "checking for win32... " >&6; }
