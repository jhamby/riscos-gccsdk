--- glib/gthread-posix.c.orig	2021-03-18 13:47:48.335693400 +0000
+++ glib/gthread-posix.c	2022-01-14 11:46:15.232532114 +0000
@@ -447,6 +447,12 @@
   if G_UNLIKELY (rwlock == NULL)
     g_thread_abort (errno, "malloc");
 
+#ifdef __riscos__
+  /* pthread_rwlock_init checks for an existing lock before initialising it,
+   * but here we have uninitalised memory.  */
+  rwlock->type = LOCK_INVALID;
+#endif
+
   if G_UNLIKELY ((status = pthread_rwlock_init (rwlock, NULL)) != 0)
     g_thread_abort (status, "pthread_rwlock_init");
 
