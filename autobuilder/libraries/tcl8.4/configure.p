--- unix/configure.orig	2006-02-25 13:34:39.000000000 +0100
+++ unix/configure	2006-02-25 13:42:41.000000000 +0100
@@ -5371,7 +5371,7 @@
 else
   
     if test "$cross_compiling" = yes; then
-  tcl_cv_api_serial=no
+  tcl_cv_api_serial=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 5378 "configure"
@@ -5403,7 +5403,7 @@
 
     if test $tcl_cv_api_serial = no ; then
 	if test "$cross_compiling" = yes; then
-  tcl_cv_api_serial=no
+  tcl_cv_api_serial=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 5410 "configure"
@@ -5435,7 +5435,7 @@
     fi
     if test $tcl_cv_api_serial = no ; then
 	if test "$cross_compiling" = yes; then
-  tcl_cv_api_serial=no
+  tcl_cv_api_serial=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 5442 "configure"
@@ -5468,7 +5468,7 @@
     fi
     if test $tcl_cv_api_serial = no ; then
 	if test "$cross_compiling" = yes; then
-  tcl_cv_api_serial=no
+  tcl_cv_api_serial=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 5475 "configure"
@@ -5503,7 +5503,7 @@
     fi
     if test $tcl_cv_api_serial = no; then
 	if test "$cross_compiling" = yes; then
-  tcl_cv_api_serial=no
+  tcl_cv_api_serial=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 5510 "configure"
@@ -5537,7 +5537,7 @@
     fi
     if test $tcl_cv_api_serial = no; then
 	if test "$cross_compiling" = yes; then
-  tcl_cv_api_serial=none
+  tcl_cv_api_serial=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 5544 "configure"
@@ -6151,7 +6151,7 @@
   echo $ac_n "(cached) $ac_c" 1>&6
 else
   if test "$cross_compiling" = yes; then
-  ac_cv_func_memcmp_clean=no
+  ac_cv_func_memcmp_clean=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 6158 "configure"
@@ -6251,7 +6251,7 @@
     echo $ac_n "checking proper strstr implementation""... $ac_c" 1>&6
 echo "configure:6253: checking proper strstr implementation" >&5
     if test "$cross_compiling" = yes; then
-  tcl_ok=no
+  tcl_ok=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 6258 "configure"
@@ -6340,7 +6340,7 @@
 fi
 
 if test "$cross_compiling" = yes; then
-  tcl_ok=0
+  tcl_ok=1
 else
   cat > conftest.$ac_ext <<EOF
 #line 6347 "configure"
@@ -6431,7 +6431,7 @@
 fi
 
 if test "$cross_compiling" = yes; then
-  tcl_ok=0
+  tcl_ok=1
 else
   cat > conftest.$ac_ext <<EOF
 #line 6438 "configure"
