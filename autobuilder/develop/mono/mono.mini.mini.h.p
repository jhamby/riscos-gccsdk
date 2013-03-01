--- mono/mini/mini.h.orig	2013-01-08 18:41:05.000000000 +0000
+++ mono/mini/mini.h	2013-01-20 20:21:51.000000000 +0000
@@ -43,7 +43,7 @@
  * can be linked into both mono and mono-sgen.
  */
 #if defined(HAVE_BOEHM_GC) || defined(HAVE_SGEN_GC)
-#error "The code in mini/ should not depend on these defines."
+#warning "The code in mini/ should not depend on these defines."
 #endif
 
 #ifndef G_LIKELY
