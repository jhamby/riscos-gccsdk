--- glib/gthread-posix.c.orig	2014-05-12 19:08:01.150040349 +0100
+++ glib/gthread-posix.c	2014-05-12 19:08:01.226040350 +0100
@@ -432,6 +432,12 @@
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
 
@@ -646,6 +652,7 @@
 
   pthread_condattr_init (&attr);
 
+#ifndef __riscos__
 #ifdef HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE_NP
 #elif defined (HAVE_PTHREAD_CONDATTR_SETCLOCK) && defined (CLOCK_MONOTONIC)
   if G_UNLIKELY ((status = pthread_condattr_setclock (&attr, CLOCK_MONOTONIC)) != 0)
@@ -653,6 +660,7 @@
 #else
 #error Cannot support GCond on your platform.
 #endif
+#endif
 
   cond = malloc (sizeof (pthread_cond_t));
   if G_UNLIKELY (cond == NULL)
@@ -886,7 +894,7 @@
     if ((status = pthread_cond_timedwait_relative_np (g_cond_get_impl (cond), g_mutex_get_impl (mutex), &ts)) == 0)
       return TRUE;
   }
-#elif defined (HAVE_PTHREAD_CONDATTR_SETCLOCK) && defined (CLOCK_MONOTONIC)
+#elif defined (HAVE_PTHREAD_CONDATTR_SETCLOCK) && defined (CLOCK_MONOTONIC) || defined (__riscos__)
   /* This is the exact check we used during init to set the clock to
    * monotonic, so if we're in this branch, timedwait() will already be
    * expecting a monotonic clock.
