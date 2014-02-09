--- src/hb-private.hh.orig	2013-11-20 19:18:31.730614891 +0000
+++ src/hb-private.hh	2014-02-08 14:28:18.810178485 +0000
@@ -149,6 +149,11 @@
 /* Misc */
 
 
+#ifdef __riscos__
+#define PACKED __attribute__((__packed__))
+#else
+#define PACKED
+#endif
 #if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
 #define _HB_BOOLEAN_EXPR(expr) ((expr) ? 1 : 0)
 #define likely(expr) (__builtin_expect (_HB_BOOLEAN_EXPR(expr), 1))
