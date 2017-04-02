--- glib/gthread-posix.c.orig	2016-10-22 06:21:26.000000000 +0100
+++ glib/gthread-posix.c	2017-04-01 20:02:42.492064631 +0100
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
 
@@ -649,6 +655,7 @@
 
   pthread_condattr_init (&attr);
 
+#ifndef __riscos__
 #ifdef HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE_NP
 #elif defined (HAVE_PTHREAD_CONDATTR_SETCLOCK) && defined (CLOCK_MONOTONIC)
   if G_UNLIKELY ((status = pthread_condattr_setclock (&attr, CLOCK_MONOTONIC)) != 0)
@@ -656,6 +663,7 @@
 #else
 #error Cannot support GCond on your platform.
 #endif
+#endif
 
   cond = malloc (sizeof (pthread_cond_t));
   if G_UNLIKELY (cond == NULL)
@@ -889,7 +897,7 @@
     if ((status = pthread_cond_timedwait_relative_np (g_cond_get_impl (cond), g_mutex_get_impl (mutex), &ts)) == 0)
       return TRUE;
   }
-#elif defined (HAVE_PTHREAD_CONDATTR_SETCLOCK) && defined (CLOCK_MONOTONIC)
+#elif defined (HAVE_PTHREAD_CONDATTR_SETCLOCK) && defined (CLOCK_MONOTONIC) || defined (__riscos__)
   /* This is the exact check we used during init to set the clock to
    * monotonic, so if we're in this branch, timedwait() will already be
    * expecting a monotonic clock.
