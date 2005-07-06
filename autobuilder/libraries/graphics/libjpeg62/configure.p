--- configure.old	2004-06-05 19:11:54.000000000 +0100
+++ configure	2004-06-05 19:12:02.000000000 +0100
@@ -1559,7 +1559,7 @@
   if test "x$LTSTATIC" = xno; then
     disable_static="--disable-static"
   fi
-  $srcdir/ltconfig $disable_shared $disable_static $srcdir/ltmain.sh
+  $srcdir/ltconfig $disable_shared $disable_static $srcdir/ltmain.sh arm-riscos-aof
 fi
 
 # Select memory manager depending on user input.
