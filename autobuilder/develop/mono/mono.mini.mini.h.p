--- mono/mini/mini.h.orig	2013-07-30 02:50:26.000000000 +0100
+++ mono/mini/mini.h	2013-08-02 20:24:44.867899125 +0100
@@ -49,7 +49,7 @@
  * can be linked into both mono and mono-sgen.
  */
 #if defined(HAVE_BOEHM_GC) || defined(HAVE_SGEN_GC)
-#error "The code in mini/ should not depend on these defines."
+#warning "The code in mini/ should not depend on these defines."
 #endif
 
 #ifndef G_LIKELY
