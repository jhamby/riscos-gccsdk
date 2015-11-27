--- src/hsts.c.orig	2015-11-27 09:52:21.091160111 +0000
+++ src/hsts.c	2015-11-27 09:52:50.583159791 +0000
@@ -502,7 +502,9 @@
         {
           /* Lock the file to avoid potential race conditions */
           fd = fileno (fp);
+#ifndef __riscos__
           flock (fd, LOCK_EX);
+#endif
 
           /* If the file has changed, merge the changes with our in-memory data
              before dumping them to the file.
