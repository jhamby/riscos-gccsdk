--- configure.orig	2004-04-18 13:51:57.000000000 +0100
+++ configure	2004-06-24 10:09:04.000000000 +0100
@@ -4861,9 +4861,9 @@
 echo "$as_me:4861: checking compiler and flags for sanity" >&5
 echo $ECHO_N "checking compiler and flags for sanity... $ECHO_C" >&6
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:4864: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+ : # { { echo "$as_me:4864: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 4869 "configure"
@@ -5968,9 +5968,9 @@
 echo "$as_me:5968: checking for zlib 1.1.4 or greater" >&5
 echo $ECHO_N "checking for zlib 1.1.4 or greater... $ECHO_C" >&6
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:5971: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:5971: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 5976 "configure"
@@ -6674,9 +6674,9 @@
 echo "$as_me:6674: checking whether struct dirent allocates space for d_name" >&5
 echo $ECHO_N "checking whether struct dirent allocates space for d_name... $ECHO_C" >&6
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:6677: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:6677: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 6682 "configure"
@@ -7816,9 +7816,9 @@
 		echo "$as_me:7816: checking if setresuid seems to work" >&5
 echo $ECHO_N "checking if setresuid seems to work... $ECHO_C" >&6
 	if test "$cross_compiling" = yes; then
-  { { echo "$as_me:7819: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:7819: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 7824 "configure"
@@ -7929,9 +7929,9 @@
 		echo "$as_me:7929: checking if setresgid seems to work" >&5
 echo $ECHO_N "checking if setresgid seems to work... $ECHO_C" >&6
 	if test "$cross_compiling" = yes; then
-  { { echo "$as_me:7932: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:7932: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 7937 "configure"
@@ -8567,9 +8567,9 @@
 	echo "$as_me:8567: checking whether snprintf correctly terminates long strings" >&5
 echo $ECHO_N "checking whether snprintf correctly terminates long strings... $ECHO_C" >&6
 	if test "$cross_compiling" = yes; then
-  { { echo "$as_me:8570: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:8570: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 8575 "configure"
@@ -9477,9 +9477,9 @@
 echo "$as_me:9477: checking OpenSSL header version" >&5
 echo $ECHO_N "checking OpenSSL header version... $ECHO_C" >&6
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:9480: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:9480: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 9485 "configure"
@@ -9539,9 +9539,9 @@
 echo "$as_me:9539: checking OpenSSL library version" >&5
 echo $ECHO_N "checking OpenSSL library version... $ECHO_C" >&6
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:9542: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:9542: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 9547 "configure"
@@ -9602,9 +9602,9 @@
 echo "$as_me:9602: checking whether OpenSSL's headers match the library" >&5
 echo $ECHO_N "checking whether OpenSSL's headers match the library... $ECHO_C" >&6
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:9605: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:9605: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 9610 "configure"
@@ -9777,9 +9777,9 @@
 echo "$as_me:9777: checking whether OpenSSL's PRNG is internally seeded" >&5
 echo $ECHO_N "checking whether OpenSSL's PRNG is internally seeded... $ECHO_C" >&6
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:9780: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:9780: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 9785 "configure"
@@ -13304,9 +13304,9 @@
 	exit 1;
 else
 	if test "$cross_compiling" = yes; then
-  { { echo "$as_me:13307: error: cannot run test program while cross compiling" >&5
-echo "$as_me: error: cannot run test program while cross compiling" >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:13307: error: cannot run test program while cross compiling" >&5
+#echo "$as_me: error: cannot run test program while cross compiling" >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line 13312 "configure"
