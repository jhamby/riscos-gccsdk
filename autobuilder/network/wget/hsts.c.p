--- src/hsts.c.orig	2017-02-05 03:22:12.000000000 +1300
+++ src/hsts.c	2017-07-26 22:18:57.124965350 +1200
@@ -560,7 +560,9 @@
         {
           /* Lock the file to avoid potential race conditions */
           fd = fileno (fp);
+#ifndef __riscos__
           flock (fd, LOCK_EX);
+#endif
 
           /* If the file has changed, merge the changes with our in-memory data
              before dumping them to the file.
