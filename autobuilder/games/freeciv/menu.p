--- client/gui-sdl/menu.c.orig	2008-06-16 08:48:04.000000000 +0100
+++ client/gui-sdl/menu.c	2008-10-15 09:09:22.296416300 +0100
@@ -573,6 +573,11 @@
   set_wstate(pBuf, FC_WS_NORMAL);
   pBuf->action = unit_order_callback;
   len = (len + 1) * sizeof(Uint16);
+#ifdef __riscos__
+  /* RISC OS iconv puts in BOM as start of string */
+  len += 2;
+#endif
+
   unibuf = fc_calloc(1, len);
   convertcopy_to_utf16(unibuf, len, cBuf);
   pBuf->string16 = create_string16(unibuf, len, adj_font(10));
@@ -812,6 +817,10 @@
   set_wstate(pBuf, FC_WS_NORMAL);
   pBuf->action = unit_order_callback;
   len = (len + 1) * sizeof(Uint16);
+#ifdef __riscos__
+  /* RISC OS iconv puts in BOM as start of string */
+  len +=2;
+#endif
   unibuf = fc_calloc(1, len);
   convertcopy_to_utf16(unibuf, len, cBuf);
   pBuf->string16 = create_string16(unibuf, len, adj_font(10));
@@ -845,6 +854,10 @@
   set_wstate(pBuf, FC_WS_NORMAL);
   pBuf->action = unit_order_callback;
   len = (len + 1) * sizeof(Uint16);
+#ifdef __riscos__
+  /* RISC OS iconv puts in BOM as start of string */
+  len +=2;
+#endif
   unibuf = fc_calloc(1, len);
   convertcopy_to_utf16(unibuf, len, cBuf);
   pBuf->string16 = create_string16(unibuf, len, adj_font(10));
