--- unix/sharing.c.orig	2017-11-05 16:54:17.434747853 +0000
+++ unix/sharing.c	2017-11-05 16:55:07.377459827 +0000
@@ -298,6 +299,7 @@
         sfree(lockname);
         return SHARE_NONE;
     }
+#ifndef __riscos__
     if (flock(lockfd, LOCK_EX) < 0) {
         *logtext = dupprintf("%s: flock(LOCK_EX): %s",
                              lockname, strerror(errno));
@@ -306,7 +308,7 @@
         close(lockfd);
         return SHARE_NONE;
     }
-
+#endif
     sockname = dupprintf("%s/socket", dirname);

     *logtext = NULL;
