--- src/fsusage.c.orig	2005-06-10 09:46:56.000000000 +0100
+++ src/fsusage.c	2005-06-10 09:48:15.000000000 +0100
@@ -308,7 +308,7 @@
 
 #endif /* STAT_STATVFS */
 
-#if !defined(STAT_STATFS2_FS_DATA) && !defined(STAT_READ_FILSYS)
+#if !defined(STAT_STATFS2_FS_DATA) && !defined(STAT_READ_FILSYS) && !defined(__riscos__)
 				/* !Ultrix && !SVR2 */
 
   fsp->fsu_blocks = CONVERT_BLOCKS (fsd.f_blocks);
