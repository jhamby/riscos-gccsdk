--- lib/Support/MemoryBuffer.cpp.orig	2014-07-06 18:43:13.000000000 +0100
+++ lib/Support/MemoryBuffer.cpp	2015-02-09 15:35:24.127549465 +0000
@@ -267,6 +267,12 @@
                           bool RequiresNullTerminator,
                           int PageSize,
                           bool IsVolatileSize) {
+#ifdef __riscos__
+  // Don't use mmap if we can help it. Unixlib only supports up to 8 mmap
+  // areas at a time and we can easily exceed that.
+  return false;
+#endif
+
   // mmap may leave the buffer without null terminator if the file size changed
   // by the time the last page is mapped in, so avoid it if the file size is
   // likely to change.
