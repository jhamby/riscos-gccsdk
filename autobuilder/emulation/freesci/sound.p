--- src/sound/pcmout_oss.c.orig	2005-07-08 10:41:45.000000000 +0100
+++ src/sound/pcmout_oss.c	2005-07-08 10:41:22.000000000 +0100
@@ -70,7 +70,7 @@
 		return -1;
 	}
 
-	if (ioctl (oss_fd, SNDCTL_DSP_RESET)) {
+	if (ioctl (oss_fd, SNDCTL_DSP_RESET, 0)) {
 		fprintf(stderr, "[PCM-OSS] Failed to reset device\n");
 		return -1;
 	}
@@ -121,7 +121,7 @@
 
   pthread_join(thread, NULL);
 
-  ioctl (oss_fd, SNDCTL_DSP_SYNC);
+  ioctl (oss_fd, SNDCTL_DSP_SYNC, 0);
   close (oss_fd);
 
   return 0;
