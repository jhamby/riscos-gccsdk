--- include/vlc_threads.h.orig	2009-08-13 07:56:48.000000000 -0700
+++ include/vlc_threads.h	2009-08-13 07:57:05.000000000 -0700
@@ -482,7 +482,7 @@
     return p_ret;
 }
 
-# if defined (_POSIX_SPIN_LOCKS) && ((_POSIX_SPIN_LOCKS - 0) > 0)
+# if defined (_POSIX_SPIN_LOCKS) && ((_POSIX_SPIN_LOCKS - 0) > 0) && !defined(__riscos__)
 typedef pthread_spinlock_t vlc_spinlock_t;
 
 /**
