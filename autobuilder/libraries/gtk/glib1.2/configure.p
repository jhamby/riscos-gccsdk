--- configure	2001-03-15 15:44:01.000000000 +0000
+++ configure	2002-08-28 15:01:44.000000000 +0100
@@ -556,7 +556,7 @@
 ac_link='${CC-cc} -o conftest${ac_exeext} $CFLAGS $CPPFLAGS $LDFLAGS conftest.$ac_ext $LIBS 1>&5'
 cross_compiling=$ac_cv_prog_cc_cross
 
-ac_exeext=
+ac_exeext=,ff8;
 ac_objext=o
 if (echo "testing\c"; echo 1,2,3) | grep c >/dev/null; then
   # Stardent Vistra SVR4 grep lacks -e, says ghazi@caip.rutgers.edu.
@@ -2099,7 +2099,7 @@
 glib_save_LIBS=$LIBS
 LIBS="$LIBS -lm"
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : # { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 2106 "configure"
@@ -5599,9 +5599,9 @@
 	echo "#endif" >>confrun.c
 done
 echo "return 0; }" >>confrun.c
-if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
-else
+#if test "$cross_compiling" = yes; then
+   #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+#else
   cat > conftest.$ac_ext <<EOF
 #line 5607 "configure"
 #include "confdefs.h"
@@ -5627,7 +5627,7 @@
 	echo "$ac_t""failed" 1>&6
 fi
 rm -fr conftest*
-fi
+#fi
 
 rm -f confrun.c
 
--- configure	2004-06-17 11:05:46.000000000 +0100
+++ configure.new	2004-06-17 11:06:26.000000000 +0100
@@ -9237,7 +9237,7 @@
 echo "configure:9238: checking whether pthread_mutex_trylock is posix like" >&5
 		# DCE Threads return 1 as success, posix 0. what a mess.
 		if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+: #    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 9244 "configure"
@@ -9267,7 +9267,7 @@
 echo "configure:9268: checking whether pthread_cond_timedwait is posix like" >&5
 		# DCE Threads return -1 as failure, posix ETIMEDOUT.
 		if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+: #    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 9274 "configure"
