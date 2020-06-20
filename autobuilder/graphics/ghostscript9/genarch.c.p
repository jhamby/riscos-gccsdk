--- base/genarch.c.orig	2012-09-25 09:53:59.000000000 +0100
+++ base/genarch.c	2012-09-25 09:55:15.000000000 +0100
@@ -31,9 +31,13 @@
    fallback to a generic int if no such type is defined.
    This default must be kept in sync with the one in gxcindex.h
    or ARCH_SIZEOF_GX_COLOR_INDEX will be incorrect for such builds. */
+#ifndef __riscos__
 #ifndef GX_COLOR_INDEX_TYPE
 #define GX_COLOR_INDEX_TYPE ulong
 #endif
+#else
+#define GX_COLOR_INDEX_TYPE unsigned long long
+#endif

 /* We should write the result on stdout, but the original Turbo C 'make' */
 /* can't handle output redirection (sigh). */
