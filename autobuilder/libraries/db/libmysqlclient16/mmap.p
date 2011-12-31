--- mysys/my_mmap.c.orig	2008-01-12 09:03:53.000000000 -0800
+++ mysys/my_mmap.c	2008-01-12 09:04:03.000000000 -0800
@@ -23,7 +23,7 @@
 */
 int my_msync(int fd, void *addr, size_t len, int flags)
 {
-  msync(addr, len, flags);
+  msync(addr, len /*, flags*/);
   return my_sync(fd, MYF(0));
 }
 
