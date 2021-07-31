--- src/main.h.orig	2010-07-17 08:29:58.000000000 +0100
+++ src/main.h	2010-07-17 08:31:02.000000000 +0100
@@ -58,7 +58,11 @@

 // data directory

+#ifdef __riscos__
+#define DATA_PREFIX "/<CrazeeSpod$Dir>/"
+#else
 #define DATA_PREFIX "./"
+#endif

 // debug messages 1 on 0 off

