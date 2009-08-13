--- drivers/drv_oss.c.orig	2004-12-21 23:41:45.000000000 +0000
+++ drivers/drv_oss.c	2004-12-21 23:41:57.000000000 +0000
@@ -310,7 +310,7 @@
 {
 	VC_PlayStop();
 
-	ioctl(sndfd,SNDCTL_DSP_POST);
+	ioctl(sndfd,SNDCTL_DSP_POST, 0);
 }
 
 static void OSS_Update(void)
@@ -350,7 +350,7 @@
 static BOOL OSS_Reset(void)
 {
 	OSS_Exit_internal();
-	ioctl(sndfd,SNDCTL_DSP_RESET);
+	ioctl(sndfd,SNDCTL_DSP_RESET, 0);
 	return OSS_Init_internal();
 }
 
