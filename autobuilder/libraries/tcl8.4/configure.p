--- unix/configure.old	2003-01-13 22:36:41.000000000 +0000
+++ unix/configure	2003-01-13 22:55:16.000000000 +0000
@@ -3615,7 +3615,7 @@
 echo $ac_n "checking proper strstr implementation""... $ac_c" 1>&6
 echo "configure:3617: checking proper strstr implementation" >&5
 if test "$cross_compiling" = yes; then
-  tcl_ok=no
+ tcl_ok=yes
 else
   cat > conftest.$ac_ext <<EOF
 #line 3622 "configure"
@@ -3703,7 +3703,7 @@
 fi
 
 if test "$cross_compiling" = yes; then
-  tcl_ok=0
+  tcl_ok=1
 else
   cat > conftest.$ac_ext <<EOF
 #line 3710 "configure"
@@ -3794,7 +3794,7 @@
 fi
 
 if test "$cross_compiling" = yes; then
-  tcl_ok=0
+  tcl_ok=1
 else
   cat > conftest.$ac_ext <<EOF
 #line 3801 "configure"
@@ -3895,7 +3895,7 @@
 else
   
 	    if test "$cross_compiling" = yes; then
-  tcl_cv_strtod_buggy=0
+  tcl_cv_strtod_buggy=1
 else
   cat > conftest.$ac_ext <<EOF
 #line 3902 "configure"
