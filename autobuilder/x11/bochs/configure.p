--- configure.old	2003-04-24 19:19:53.000000000 +0100
+++ configure	2003-04-24 18:47:00.000000000 +0100
@@ -2087,7 +2087,7 @@
 
 # Be careful to initialize this variable, since it used to be cached.
 # Otherwise an old cache value of `no' led to `EXEEXT = no' in a Makefile.
-ac_cv_exeext=
+ac_cv_exeext=,ff8;
 # b.out is created by i960 compilers.
 for ac_file in a_out.exe a.exe conftest.exe a.out conftest a.* conftest.* b.out
 do
@@ -3513,7 +3513,7 @@
   (eval $ac_link) 2>&5
   ac_status=$?
   echo "$as_me:$LINENO: \$? = $ac_status" >&5
-  (exit $ac_status); } && test -s conftest; then
+  (exit $ac_status); } && test -s conftest${ac_exeext}; then
 	    pipe_works=yes
 	  fi
 	  LIBS="$save_LIBS"
@@ -21548,11 +21548,11 @@
 OLD_LIBS=$LIBS
 LIBS="$LIBS -lreadline"
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&5
-echo "$as_me: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&5
+#echo "$as_me: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line $LINENO "configure"
@@ -21597,11 +21597,11 @@
 echo $ECHO_N "checking if readline works with -lcurses... $ECHO_C" >&6
 LIBS="$LIBS -lcurses"
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&5
-echo "$as_me: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&5
+#echo "$as_me: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line $LINENO "configure"
