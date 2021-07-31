--- memtester.c.orig	2009-08-02 20:15:19.000000000 -0700
+++ memtester.c	2009-08-02 20:16:03.000000000 -0700
@@ -26,6 +26,9 @@
 #include "sizes.h"
 #include "tests.h"
 
+#define mlock(addr, len) 0
+#define munlock(addr, len) 0
+
 #define EXIT_FAIL_NONSTARTER    0x01
 #define EXIT_FAIL_ADDRESSLINES  0x02
 #define EXIT_FAIL_OTHERTEST     0x04
