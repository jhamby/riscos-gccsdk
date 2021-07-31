--- default.c.orig	2008-03-08 18:08:37.000000000 -0800
+++ default.c	2008-03-08 18:14:12.000000000 -0800
@@ -390,7 +390,11 @@
 
     "AR", "ar",
     "ARFLAGS", "rv",
+#ifdef __riscos__
+    "AS", "gcc",         /* Always call assembler via GCC */
+#else
     "AS", "as",
+#endif
 #ifdef GCC_IS_NATIVE
     "CC", "gcc",
 # ifdef __MSDOS__
