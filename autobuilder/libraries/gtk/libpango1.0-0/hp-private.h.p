--- pango/opentype/hb-private.h.orig	2009-11-17 16:35:44.000000000 +0000
+++ pango/opentype/hb-private.h	2009-12-23 21:07:51.000000000 +0000
@@ -130,6 +130,11 @@
 
 #define ASSERT_SIZE(_type, _size) ASSERT_STATIC (sizeof (_type) == (_size))
 
+#ifdef __riscos__
+#define PACKED __attribute__((__packed__))
+#else
+#define PACKED
+#endif
 
 #if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
 #define _HB_BOOLEAN_EXPR(expr)                   \
