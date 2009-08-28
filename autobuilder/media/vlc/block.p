--- src/misc/block.c.orig	2009-08-28 09:05:05.000000000 -0700
+++ src/misc/block.c	2009-08-28 09:17:09.000000000 -0700
@@ -271,6 +271,21 @@
 }
 #endif
 
+#ifdef __riscos__
+ssize_t pread (int fd, void *buf, size_t count, off_t offset)
+{
+  off_t cur = lseek(fd, 0, SEEK_CUR);
+  ssize_t res;
+
+  if (cur == -1 || lseek(fd, offset, SEEK_SET) == -1) return -1;
+
+  res = read(fd, buf, count);
+
+  lseek(fd, cur, SEEK_SET);
+  return res;
+}
+#endif
+
 /**
  * Loads a file into a block of memory. If possible a private file mapping is
  * created. Otherwise, the file is read normally. On 32-bits platforms, this
