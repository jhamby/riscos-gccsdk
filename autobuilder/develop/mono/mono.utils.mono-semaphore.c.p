--- mono/utils/mono-semaphore.c.orig	2013-01-08 18:41:09.000000000 +0000
+++ mono/utils/mono-semaphore.c	2013-01-18 20:25:38.000000000 +0000
@@ -17,6 +17,10 @@
 #include <unistd.h>
 #endif
 
+#ifdef __riscos__
+int sem_timedwait (sem_t *sem, const struct timespec *abstime);
+#endif
+
 #if (defined (HAVE_SEMAPHORE_H) || defined (USE_MACH_SEMA)) && !defined(HOST_WIN32)
 /* sem_* or semaphore_* functions in use */
 #  ifdef USE_MACH_SEMA
