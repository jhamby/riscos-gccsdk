--- config.h.in.orig	2009-08-03 11:28:09.000000000 -0700
+++ config.h.in	2009-08-03 11:28:24.000000000 -0700
@@ -781,7 +781,7 @@
 #undef HAVE_SETRLIMIT
 
 /* Define to 1 if you have the `setsid' function. */
-#undef HAVE_SETSID
+#define HAVE_SETSID 1
 
 /* Define to 1 if you have the `setutent' function. */
 #undef HAVE_SETUTENT
