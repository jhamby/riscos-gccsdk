--- base/gxcindex.h.orig	2012-09-25 09:46:27.000000000 +0100
+++ base/gxcindex.h	2012-09-25 09:47:52.000000000 +0100
@@ -51,6 +51,7 @@
 #else  /* !TEST_CINDEX_STRUCT */

 /* Define the type for device color index (pixel value) data. */
+#ifndef __riscos__
 #ifdef GX_COLOR_INDEX_TYPE
 typedef GX_COLOR_INDEX_TYPE gx_color_index_data;
 #else
@@ -58,6 +59,9 @@
    or ARCH_SIZEOF_GX_COLOR_INDEX will be incorrect */
 typedef ulong gx_color_index_data;
 #endif
+#else
+typedef unsigned long long gx_color_index_data;
+#endif

 #endif /* (!)TEST_CINDEX_STRUCT */

