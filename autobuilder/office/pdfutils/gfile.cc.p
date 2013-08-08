--- goo/gfile.cc.orig	2013-05-23 16:42:45.240000000 +0100
+++ goo/gfile.cc	2013-05-23 16:54:46.711061462 +0100
@@ -53,6 +53,8 @@
 #endif // _WIN32
 #include <stdio.h>
 #include <limits>
+#include <errno.h>
+#include <sys/types.h>
 #include "GooString.h"
 #include "gfile.h"

@@ -62,6 +62,11 @@
 #define PATH_MAX 1024
 #endif

+typedef long _ssize_t;
+
+static ssize_t goo_pread(int fd, void *buf, size_t count, off_t offset);
+static ssize_t read_in_full(int fd, void *buf, size_t count);
+
 //------------------------------------------------------------------------

 GooString *getCurrentDir() {
@@ -641,7 +647,7 @@
 #ifdef HAVE_PREAD64
   return pread64(fd, buf, n, offset);
 #else
-  return pread(fd, buf, n, offset);
+  return goo_pread(fd, buf, n, offset);
 #endif
 }

@@ -807,3 +813,54 @@
 #endif
 #endif
 }
+
+/*
+ * xread() is the same a read(), but it automatically restarts read()
+ * operations with a recoverable error (EAGAIN and EINTR). xread()
+ * DOES NOT GUARANTEE that "len" bytes is read even if the data is available.
+ */
+static ssize_t xread(int fd, void *buf, size_t len)
+{
+        ssize_t nr;
+        while (1) {
+                nr = read(fd, buf, len);
+                if ((nr < 0) && (errno == EAGAIN || errno == EINTR))
+                        continue;
+                return nr;
+        }
+}
+
+static ssize_t read_in_full(int fd, void *buf, size_t count)
+{
+        char *p = (char *) buf;
+        ssize_t total = 0;
+
+        while (count > 0) {
+                ssize_t loaded = xread(fd, p, count);
+                if (loaded < 0)
+                        return -1;
+                if (loaded == 0)
+                        return total;
+                count -= loaded;
+                p += loaded;
+                total += loaded;
+        }
+
+        return total;
+}
+static ssize_t goo_pread(int fd, void *buf, size_t count, off_t offset)
+{
+        off_t current_offset;
+        ssize_t rc;
+
+        current_offset = lseek(fd, 0, SEEK_CUR);
+
+        if (lseek(fd, offset, SEEK_SET) < 0)
+                return -1;
+
+        rc = read_in_full(fd, buf, count);
+
+        if (current_offset != lseek(fd, current_offset, SEEK_SET))
+                return -1;
+        return rc;
+}
