--- config.guess.orig	2006-09-24 23:20:22.000000000 +0200
+++ config.guess	2006-09-24 23:30:49.000000000 +0200
@@ -1297,6 +1297,10 @@
   printf ("arm-acorn-riscix\n"); exit (0);
 #endif
 
+#if defined (__arm__) && defined (__riscos__)
+  printf ("arm-unknown-riscos\n"); exit (0);
+#endif
+
 #if defined (hp300) && !defined (hpux)
   printf ("m68k-hp-bsd\n"); exit (0);
 #endif
