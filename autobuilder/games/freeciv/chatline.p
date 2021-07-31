--- client/gui-sdl/chatline.c.orig	2008-06-16 08:48:05.000000000 +0100
+++ client/gui-sdl/chatline.c	2008-10-15 09:07:23.553416300 +0100
@@ -418,6 +418,10 @@
     size_t n = strlen(astring);
     
     n += 1;
+#ifdef __riscos__
+    /* RISC OS iconv starts string with BOM */
+    n += 1;
+#endif
     n *= 2;
     pUniStr = fc_calloc(1, n);
     convertcopy_to_utf16(pUniStr, n, astring);
