--- pango/opentype/hb-private.h.orig	2009-11-26 00:44:17.000000000 +0000
+++ pango/opentype/hb-private.h	2009-12-27 11:34:25.000000000 +0000
@@ -99,6 +99,11 @@
 #define hb_be_int32_get_unaligned	(int32_t) hb_be_uint32_get_unaligned
 #define hb_be_int32_cmp_unaligned	hb_be_uint32_cmp_unaligned
 
+#ifdef __riscos__
+#define PACKED __attribute__((__packed__))
+#else
+#define PACKED
+#endif
 
 /* Basics */
 
