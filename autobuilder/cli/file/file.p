--- src/file.h.old	2004-09-27 07:35:13.000000000 +0100
+++ src/file.h	2004-09-27 07:35:08.000000000 +0100
@@ -59,6 +59,8 @@
 
 #ifdef __EMX__
 #define PATHSEP	';'
+#elif __riscos__
+#define PATHSEP ','
 #else
 #define PATHSEP	':'
 #endif
