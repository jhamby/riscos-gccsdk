--- configure.org	2010-01-10 18:03:32.000000000 +0100
+++ configure	2010-01-10 18:01:05.000000000 +0100
@@ -180,11 +180,11 @@
   esac
 fi
 
+
+if test $shared -eq 1; then
 SHAREDLIB=${SHAREDLIB-"libz$shared_ext"}
 SHAREDLIBV=${SHAREDLIBV-"libz$shared_ext.$VER"}
 SHAREDLIBM=${SHAREDLIBM-"libz$shared_ext.$VER1"}
-
-if test $shared -eq 1; then
   echo Checking for shared library support...
   # we must test in two steps (cc then ld), required at least on SunOS 4.x
   if test "`($CC -w -c $SFLAGS $test.c) 2>&1`" = "" &&
@@ -220,18 +220,20 @@
 
 EOF
 
-cat > $test.c <<EOF
-#include <sys/types.h>
-off64_t dummy = 0;
-EOF
-if test "`($CC -c $CFLAGS -D_LARGEFILE64_SOURCE=1 $test.c) 2>&1`" = ""; then
-  CFLAGS="${CFLAGS} -D_LARGEFILE64_SOURCE=1"
-  SFLAGS="${SFLAGS} -D_LARGEFILE64_SOURCE=1"
-  ALL="${ALL} all64"
-  TEST="${TEST} test64"
-  echo "Checking for off64_t... Yes."
-  echo "Checking for fseeko... Yes."
-else
+# Stop LARGEFILE64_SOURCE definition for RISC OS as the current
+# version of gcc doesn't support fopen64 or fseeko64
+#cat > $test.c <<EOF
+##include <sys/types.h>
+#off64_t dummy = 0;
+#EOF
+#if test "`($CC -c $CFLAGS -D_LARGEFILE64_SOURCE=1 $test.c) 2>&1`" = ""; then
+#  CFLAGS="${CFLAGS} -D_LARGEFILE64_SOURCE=1"
+#  SFLAGS="${SFLAGS} -D_LARGEFILE64_SOURCE=1"
+#  ALL="${ALL} all64"
+#  TEST="${TEST} test64"
+#  echo "Checking for off64_t... Yes."
+#  echo "Checking for fseeko... Yes."
+#else
   echo "Checking for off64_t... No."
   cat > $test.c <<EOF
 #include <stdio.h>
@@ -247,7 +249,7 @@
     SFLAGS="${SFLAGS} -DNO_FSEEKO"
     echo "Checking for fseeko... No."
   fi
-fi
+#fi
 
 cat > $test.c <<EOF
 #include <unistd.h>
