--- configure.orig	2012-05-02 05:17:08.000000000 +0100
+++ configure	2012-06-08 13:24:31.612028800 +0100
@@ -385,14 +385,16 @@
 #include <sys/types.h>
 off64_t dummy = 0;
 EOF
-if try $CC -c $CFLAGS -D_LARGEFILE64_SOURCE=1 $test.c; then
-  CFLAGS="${CFLAGS} -D_LARGEFILE64_SOURCE=1"
-  SFLAGS="${SFLAGS} -D_LARGEFILE64_SOURCE=1"
-  ALL="${ALL} all64"
-  TEST="${TEST} test64"
-  echo "Checking for off64_t... Yes." | tee -a configure.log
-  echo "Checking for fseeko... Yes." | tee -a configure.log
-else
+# Stop LARGEFILE_SOURCE definition for RISC OS as teh current
+# version of gcc doesn't support fopen64 or fseeko64
+#if try $CC -c $CFLAGS -D_LARGEFILE64_SOURCE=1 $test.c; then
+#  CFLAGS="${CFLAGS} -D_LARGEFILE64_SOURCE=1"
+#  SFLAGS="${SFLAGS} -D_LARGEFILE64_SOURCE=1"
+#  ALL="${ALL} all64"
+#  TEST="${TEST} test64"
+#  echo "Checking for off64_t... Yes." | tee -a configure.log
+#  echo "Checking for fseeko... Yes." | tee -a configure.log
+#else
   echo "Checking for off64_t... No." | tee -a configure.log
   echo >> configure.log
   cat > $test.c <<EOF
@@ -409,7 +411,7 @@
     SFLAGS="${SFLAGS} -DNO_FSEEKO"
     echo "Checking for fseeko... No." | tee -a configure.log
   fi
-fi
+#fi
 
 echo >> configure.log
 
@@ -716,13 +718,15 @@
 }
 EOF
 Z_U4=""
-if try $CC $CFLAGS $test.c -o $test && Z_U4=`./$test` && test -n "$Z_U4"; then
+# RISC OS autobuilder cross-compiles so can't run a test program
+Z_U4="unsigned"
+# if try $CC $CFLAGS $test.c -o $test && Z_U4=`./$test` && test -n "$Z_U4"; then
   sed < zconf.h "/#define Z_U4/s/\/\* \.\/configure may/#define Z_U4 $Z_U4   \/* .\/configure put the/" > zconf.temp.h
   mv zconf.temp.h zconf.h
   echo "Looking for a four-byte integer type... Found." | tee -a configure.log
-else
-  echo "Looking for a four-byte integer type... Not found." | tee -a configure.log
-fi
+#else
+#  echo "Looking for a four-byte integer type... Not found." | tee -a configure.log
+#fi
 
 # clean up files produced by running the compiler and linker
 rm -f $test.[co] $test $test$shared_ext $test.gcno
