--- include/c.h.orig	2021-10-02 10:09:05.491610647 +0100
+++ include/c.h	2021-10-02 10:08:01.594294807 +0100
@@ -307,7 +307,11 @@
  */
 static inline size_t get_hostname_max(void)
 {
+#ifndef __riscos__
 	long len = sysconf(_SC_HOST_NAME_MAX);
+#else
+	long len = 255;
+#endif
 
 	if (0 < len)
 		return len;
