--- configure.orig	2008-09-01 16:39:32.832735300 +0100
+++ configure	2008-09-01 16:40:45.706735300 +0100
@@ -5234,9 +5234,10 @@
 	fi
 fi
 
-if test "$ARCH" = "linux"; then
-	LDFLAGS="$LDFLAGS -ldl"
-fi
+# RISC OS appears a linux but doesn't want the dl library
+#if test "$ARCH" = "linux"; then
+#	LDFLAGS="$LDFLAGS -ldl"
+#fi
 if test "${ac_cv_header_lua_h+set}" = set; then
   echo "$as_me:$LINENO: checking for lua.h" >&5
 echo $ECHO_N "checking for lua.h... $ECHO_C" >&6
