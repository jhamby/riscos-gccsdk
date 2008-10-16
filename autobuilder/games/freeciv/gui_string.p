--- client/gui-sdl/gui_string.c.orig	2008-06-16 08:48:06.000000000 +0100
+++ client/gui-sdl/gui_string.c	2008-10-15 09:08:22.155416300 +0100
@@ -199,6 +199,11 @@
   assert(pCharString != NULL);
   
   n = (strlen(pCharString) + 1) * 2;
+#ifdef __riscos__
+  /* RISC OS iconv puts in BOM as start of string */
+  n += 2;
+#endif
+
   
   if (n > pString->n_alloc) {
     /* allocated more if this is only a small increase on before: */
@@ -411,11 +416,30 @@
   if(pText && pText->w > w - 4) {
     /* cut string length to w length by replacing space " " with new line "\n" */
     int ptsize = pString->ptsize;
-    Uint16 pNew_Line[2], pSpace[2];
+
     Uint16 *ptr = pString->text;
-    
+
+#ifdef __riscos__
+    /* RISC OS iconv puts in BOM as start of string */
+    Uint16 pNew_Line_Buf[3], pSpace_Buf[3];
+    Uint16 *pNew_Line = pNew_Line_Buf, *pSpace = pSpace_Buf;
+
+    convertcopy_to_utf16(pNew_Line_Buf, sizeof(pNew_Line_Buf), "\n");
+    convertcopy_to_utf16(pSpace_Buf, sizeof(pSpace_Buf), " ");
+
+    /* Check for BOM to be sure */
+    if (*pNew_Line == 0xFFFE || *pNew_Line == 0xFEFF)
+    {
+       pNew_Line++;
+       pSpace++;
+    }
+
+#else
+    Uint16 pNew_Line[2], pSpace[2];
+
     convertcopy_to_utf16(pNew_Line, sizeof(pNew_Line), "\n");
     convertcopy_to_utf16(pSpace, sizeof(pSpace), " ");
+#endif
    
     do {
       if (*ptr) {
@@ -467,11 +491,25 @@
     Uint16 *ptr_rev, *ptr = pString->text;
         
     {
+#ifdef __riscos__
+      /* RISC OS iconv puts in BOM as start of string */
+      Uint16 pBuf[3];
+#else
       Uint16 pBuf[2];
+#endif
       convertcopy_to_utf16(pBuf, sizeof(pBuf), "\n");
       New_Line = pBuf[0];
+#ifdef __riscos__
+      /* RISC OS iconv puts in BOM as start of string */
+      if (New_Line == 0xFFFE || New_Line == 0xFEFF) New_Line = pBuf[1];
+#endif
+
       convertcopy_to_utf16(pBuf, sizeof(pBuf), " ");
       Space = pBuf[0];
+#ifdef __riscos__
+      /* RISC OS iconv puts in BOM as start of string */
+      if (Space == 0xFFFE || Space == 0xFEFF) Space = pBuf[1];
+#endif
     }
     
     converted = TRUE;
