--- png.h.orig	2010-06-04 21:57:26.101027558 +0100
+++ png.h	2010-06-04 21:57:48.701025951 +0100
@@ -1299,7 +1299,7 @@
    png_uint_32 num_rows PNG_DEPSTRUCT;      /* number of rows in current pass */
    png_uint_32 usr_width PNG_DEPSTRUCT;     /* width of row at start of write */
    png_uint_32 rowbytes PNG_DEPSTRUCT;      /* size of row in bytes */
-#if 0 /* Replaced with the following in libpng-1.2.43 */
+#ifdef __riscos__  /* Replaced with the following in libpng-1.2.43 */
    png_size_t irowbytes PNG_DEPSTRUCT;
 #endif
 /* Added in libpng-1.2.43 */
