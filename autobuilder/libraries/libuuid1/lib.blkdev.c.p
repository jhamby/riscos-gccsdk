--- lib/blkdev.c.orig	2021-10-02 10:13:29.992483531 +0100
+++ lib/blkdev.c	2021-10-02 10:14:06.921118241 +0100
@@ -366,7 +366,7 @@
 int blkdev_lock(int fd, const char *devname, const char *lockmode)
 {
 	int oper, rc, msg = 0;
-
+#ifndef __riscos__
 	if (!lockmode)
 		lockmode = getenv("LOCK_BLOCK_DEVICE");
 	if (!lockmode)
@@ -409,6 +409,7 @@
 		}
 	} else if (msg)
 		fprintf(stderr, _("OK\n"));
+#endif
 	return rc;
 }
 
