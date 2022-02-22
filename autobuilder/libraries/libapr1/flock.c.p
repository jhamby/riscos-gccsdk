--- file_io/unix/flock.c.orig	2021-10-19 17:53:16.270182487 +0100
+++ file_io/unix/flock.c	2021-10-19 17:54:40.688630906 +0100
@@ -27,7 +27,7 @@
 {
     int rc;
 
-#if 0 && defined(HAVE_FCNTL_H)
+#if 0 && defined(HAVE_FCNTL_H) && !defined(__riscos__)
     {
         struct flock l = { 0 };
         int fc;
@@ -67,11 +67,13 @@
             ltype = LOCK_EX;
         if ((type & APR_FLOCK_NONBLOCK) != 0)
             ltype |= LOCK_NB;
-
+#ifndef __riscos__
         /* keep trying if flock() gets interrupted (by a signal) */
         while ((rc = flock(thefile->filedes, ltype)) < 0 && errno == EINTR)
             continue;
-
+#else
+	rc = 0;
+#endif
         if (rc == -1)
             return errno;
     }
@@ -107,10 +107,13 @@
     }
 #elif defined(HAVE_SYS_FILE_H)
     {
+#ifndef __riscos__
         /* keep trying if flock() gets interrupted (by a signal) */
         while ((rc = flock(thefile->filedes, LOCK_UN)) < 0 && errno == EINTR)
             continue;
-
+#else
+	rc = 0;
+#endif
         if (rc == -1)
             return errno;
     }
