--- binutils/config.in.orig	2006-02-14 09:59:10.000000000 +0100
+++ binutils/config.in	2009-02-01 17:14:18.000000000 +0100
@@ -120,6 +120,12 @@
 /* Define to 1 if you have the `putenv' function. */
 #undef HAVE_PUTENV
 
+/* Define to 1 if you have the `mkdtemp' function. */
+#undef HAVE_MKDTEMP
+
+/* Define to 1 if you have the `mkstemp' function. */
+#undef HAVE_MKSTEMP
+
 /* Define to 1 if you have the `sbrk' function. */
 #undef HAVE_SBRK
 
