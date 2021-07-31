--- client/gui-sdl/gui_iconv.c.orig	2008-06-16 08:48:06.000000000 +0100
+++ client/gui-sdl/gui_iconv.c	2008-10-15 10:13:35.125416300 +0100
@@ -101,8 +101,13 @@
   
   if (!pResult) {
     /* From 8 bit code to UTF-16 (16 bit code) */
+#ifdef __riscos__
+    /* RISC OS iconv puts in BOM as start of string */
+    ulength = length * 2 + 2;
+#else    
     ulength = length * 2;
-    pResult = fc_calloc(1, ulength);
+#endif
+    pResult = fc_calloc(1, ulength);    
   }
 
   iconv(cd, NULL, NULL, NULL, NULL);	/* return to the initial state */
