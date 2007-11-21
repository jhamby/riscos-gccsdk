--- configure.orig	2007-11-07 09:39:25.111922700 +0000
+++ configure	2007-11-07 09:42:47.386922700 +0000
@@ -224,16 +224,18 @@
 
 EOF
 
-cat > $test.c <<EOF
-#include <sys/types.h>
-off64_t dummy = 0;
-EOF
-if test "`($CC -c $CFLAGS -D_LARGEFILE64_SOURCE=1 $test.c) 2>&1`" = ""; then
-  CFLAGS="${CFLAGS} -D_LARGEFILE64_SOURCE=1"
-  SFLAGS="${SFLAGS} -D_LARGEFILE64_SOURCE=1"
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
+#  echo "Checking for off64_t... Yes."
+#  echo "Checking for fseeko... Yes."
+#else
   echo "Checking for off64_t... No."
   cat > $test.c <<EOF
 #include <stdio.h>
@@ -249,7 +251,7 @@
     SFLAGS="${SFLAGS} -DNO_FSEEKO"
     echo "Checking for fseeko... No."
   fi
-fi
+#fi
 
 cat > $test.c <<EOF
 #include <unistd.h>
