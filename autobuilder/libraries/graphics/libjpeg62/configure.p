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
--- configure.orig	2006-12-16 15:39:07.000000000 -0800
+++ configure	2006-12-16 15:39:18.000000000 -0800
@@ -641,9 +641,9 @@
 rm -fr conftest*
 
 echo "$ac_t""$ac_cv_prog_cc_works" 1>&6
-if test $ac_cv_prog_cc_works = no; then
-  { echo "configure: error: installation or configuration problem: C compiler cannot create executables." 1>&2; exit 1; }
-fi
+#if test $ac_cv_prog_cc_works = no; then
+#  { echo "configure: error: installation or configuration problem: C compiler cannot create executables." 1>&2; exit 1; }
+#fi
 echo $ac_n "checking whether the C compiler ($CC $CFLAGS $LDFLAGS) is a cross-compiler""... $ac_c" 1>&6
 echo "configure:649: checking whether the C compiler ($CC $CFLAGS $LDFLAGS) is a cross-compiler" >&5
 echo "$ac_t""$ac_cv_prog_cc_cross" 1>&6
