--- src/network_write.c.orig	2022-11-30 13:04:18.258486518 +0000
+++ src/network_write.c	2022-11-30 13:05:13.603700142 +0000
@@ -350,6 +350,8 @@
 #elif defined(_XOPEN_IOV_MAX)
 /* minimum value for sysconf(_SC_IOV_MAX); posix requires this to be at least 16, which is good enough - no need to call sysconf() */
 # define SYS_MAX_CHUNKS _XOPEN_IOV_MAX
+#elif defined(__riscos__)
+# define SYS_MAX_CHUNKS 16
 #else
 # error neither UIO_MAXIOV nor IOV_MAX nor _XOPEN_IOV_MAX are defined
 #endif
