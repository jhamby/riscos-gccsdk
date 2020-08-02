--- glib/gthread-posix.c.orig	2016-10-22 06:21:26.000000000 +0100
+++ glib/gthread-posix.c	2019-05-25 20:46:11.310795799 +0100
@@ -439,6 +439,12 @@
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
 
