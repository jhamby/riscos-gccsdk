--- zlib.h	2009-12-24 07:09:43.000000000 -0800
+++ zlib.h.orig	2010-01-08 14:35:17.000000000 -0800
@@ -1443,7 +1443,7 @@
    ZEXTERN uLong ZEXPORT crc32_combine64 OF((uLong, uLong, off64_t));
 #endif
 
-#if !defined(ZLIB_INTERNAL) && _FILE_OFFSET_BITS == 64
+#if !defined(ZLIB_INTERNAL) && defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64
 #  define gzopen gzopen64
 #  define gzseek gzseek64
 #  define gztell gztell64
