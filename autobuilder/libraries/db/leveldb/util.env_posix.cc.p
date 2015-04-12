--- util/env_posix.cc.orig	2014-06-19 10:57:27.000000000 +0100
+++ util/env_posix.cc	2015-04-10 13:24:21.672007370 +0100
@@ -68,6 +68,30 @@
   }
 };
 
+#ifdef __riscos__
+
+static size_t
+pread(int fd, void *buf, size_t count, off_t offset)
+{
+  off_t lseek_result;
+  off_t original_pos;
+  size_t read_result;
+
+  original_pos = lseek (fd, 0, SEEK_CUR);
+
+  lseek_result = lseek (fd, offset, SEEK_SET);
+  if (lseek_result == (off_t)-1)
+    return (size_t)lseek_result;
+  read_result = read (fd, buf, count);
+
+  // Return to original file position.
+  lseek (fd, original_pos, SEEK_SET);
+
+  return read_result;
+}
+
+#endif
+
 // pread() based random-access
 class PosixRandomAccessFile: public RandomAccessFile {
  private:
