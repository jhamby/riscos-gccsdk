--- openbsd-compat/getrrsetbyname.c.orig	2015-10-21 18:17:52.932080456 +0100
+++ openbsd-compat/getrrsetbyname.c	2015-10-21 18:20:46.772078569 +0100
@@ -109,6 +109,12 @@
 }
 #endif
 
+static int my_res_init() {
+    return 0;
+}
+
+#define res_init my_res_init
+
 /*
  * Routines to insert/extract short/long's.
  */
