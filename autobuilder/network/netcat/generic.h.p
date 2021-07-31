--- generic.h	2003-04-25 07:42:16.000000000 +0100
+++ generic.h.old	2003-04-25 08:42:10.000000000 +0100
@@ -360,6 +360,13 @@
 #undef HAVE_SELECT_X
 #endif /* NeXTSTEP 3.2 motorola */
 
+#ifdef RISCOS
+#undef UTMPX
+#undef HAVE_SYSINFO
+#define HAVE_SELECT_H
+#define HAVE_TTYENT_H
+#endif
+
 /* Make some "generic" assumptions if all else fails */
 #ifdef GENERIC
 #undef HAVE_FLOCK
