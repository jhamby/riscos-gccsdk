--- git-compat-util.h.orig	2015-11-15 14:38:22.910581000 +0100
+++ git-compat-util.h	2015-11-15 14:39:13.678581000 +0100
@@ -943,4 +943,9 @@
 #define getc_unlocked(fh) getc(fh)
 #endif
 
+#ifdef __riscos
+static inline pid_t getpgid(pid_t pid)
+{ return pid == 0 ? getpid() : pid; }
+#endif
+
 #endif
