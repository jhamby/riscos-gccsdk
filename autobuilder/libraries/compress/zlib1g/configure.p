--- configure.orig	2013-03-24 05:30:09.000000000 +0000
+++ configure	2018-07-01 12:26:23.228773998 +0100
@@ -427,14 +427,16 @@
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
+# Stop LARGEFILE_SOURCE definition for RISC OS as the current
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
@@ -451,7 +453,7 @@
     SFLAGS="${SFLAGS} -DNO_FSEEKO"
     echo "Checking for fseeko... No." | tee -a configure.log
   fi
-fi
+#fi
 
 echo >> configure.log
 
