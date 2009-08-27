--- zlib.h.orig	2009-03-27 01:06:27.000000000 +0000
+++ zlib.h	2009-03-27 01:06:50.000000000 +0000
@@ -1369,7 +1369,7 @@
    ZEXTERN uLong ZEXPORT crc32_combine64 OF((uLong, uLong, off64_t));
 #endif
 
-#if _FILE_OFFSET_BITS == 64
+#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
 #  define gzopen gzopen64
 #  define gzseek gzseek64
 #  define gztell gztell64
