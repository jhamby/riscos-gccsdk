--- dist/configure	2003-01-06 22:29:18.000000000 +0000
+++ dist/configure	2002-07-07 18:52:02.000000000 +0100
@@ -543,7 +543,7 @@
 ac_link='${CC-cc} -o conftest${ac_exeext} $CFLAGS $CPPFLAGS $LDFLAGS conftest.$ac_ext $LIBS 1>&5'
 cross_compiling=$ac_cv_prog_cc_cross

-ac_exeext=
+ac_exeext=,ff8; ac_cv_exeexct=,ff8
 ac_objext=o
 if (echo "testing\c"; echo 1,2,3) | grep c >/dev/null; then
   # Stardent Vistra SVR4 grep lacks -e, says ghazi@caip.rutgers.edu.
@@ -3278,7 +3278,7 @@
 else
   db_cv_alignp_t=$db_cv_align_t
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  db_cv_alignp_t="unsigned int"; #{ echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3285 "configure"
@@ -3296,7 +3296,7 @@
 fi

 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+    : #{ echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3303 "configure"
@@ -3314,7 +3314,7 @@
 fi

 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3321 "configure"
@@ -3565,7 +3565,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  db_cv_mutex=no; #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3572 "configure"
@@ -3597,7 +3597,7 @@
 if test "$db_cv_mutex" = no -o "$db_cv_mutex" = "ui_only"; then
 LIBS="-lthread $LIBS"
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  db_cv_mutex=no; # { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3604 "configure"
@@ -3630,7 +3630,7 @@
 fi
 if test "$db_cv_mutex" = no -o "$db_cv_mutex" = "ui_only"; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  db_cv_mutex=no; #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3637 "configure"
@@ -3667,7 +3667,7 @@

 if test "$db_cv_mutex" = no -o "$db_cv_mutex" = "posix_only"; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+    : #{ echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3674 "configure"
@@ -3708,7 +3708,7 @@
 if test "$db_cv_mutex" = no -o "$db_cv_mutex" = "posix_only"; then
 LIBS="-lpthread $LIBS"
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3715 "configure"
@@ -3753,7 +3753,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3760 "configure"
@@ -3787,7 +3787,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3794 "configure"
@@ -3840,7 +3840,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3847 "configure"
@@ -3948,7 +3948,7 @@

 if test "$db_cv_mutex" = DOESNT_WORK; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3955 "configure"
@@ -3975,7 +3975,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 3982 "configure"
@@ -4002,7 +4002,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 4009 "configure"
@@ -4029,7 +4029,7 @@

 if test "$db_cv_mutex" = DOESNT_WORK; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 4036 "configure"
@@ -4057,7 +4057,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 4064 "configure"
@@ -4085,7 +4085,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+ : #   { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 4092 "configure"
@@ -4113,7 +4113,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+  : #  { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 4120 "configure"
@@ -4141,7 +4141,7 @@

 if test "$db_cv_mutex" = no; then
 if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+ : #   { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 4148 "configure"
@@ -4531,7 +4531,7 @@
   echo $ac_n "(cached) $ac_c" 1>&6
 else
   if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+    db_cv_fcntl_f_setfd=yes :  #{ echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 4538 "configure"
@@ -4687,7 +4687,7 @@
   echo $ac_n "(cached) $ac_c" 1>&6
 else
   if test "$cross_compiling" = yes; then
-    { echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
+    db_cv_sprintf_count=yes; #{ echo "configure: error: can not run test program while cross compiling" 1>&2; exit 1; }
 else
   cat > conftest.$ac_ext <<EOF
 #line 4694 "configure"
--- dist/configure.in.old	2003-04-25 13:06:02.000000000 +0100
+++ dist/configure.in	2003-04-25 12:06:00.000000000 +0100
@@ -87,7 +87,7 @@
 dnl
 dnl XXX
 dnl Don't override anything if it's already set from the environment.
-optimize_def="-O"
+optimize_def="-O2"
 case "$host_os" in
 aix4.*)	   optimize_def="-O2"
 	   CC=${CC-"xlc_r"}
--- dist/configure.old	2003-04-25 13:17:07.000000000 +0100
+++ dist/configure	2003-04-25 12:19:44.000000000 +0100
@@ -974,7 +974,7 @@
 fi


-optimize_def="-O"
+optimize_def="-O2"
 case "$host_os" in
 aix4.*)	   optimize_def="-O2"
 	   CC=${CC-"xlc_r"}
