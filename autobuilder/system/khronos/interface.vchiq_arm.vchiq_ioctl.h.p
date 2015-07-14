--- interface/vchiq_arm/vchiq_ioctl.h.orig	2015-06-18 16:41:33.312452522 +0100
+++ interface/vchiq_arm/vchiq_ioctl.h	2015-05-20 15:29:12.000000000 +0100
@@ -28,7 +28,11 @@
 #ifndef VCHIQ_IOCTLS_H
 #define VCHIQ_IOCTLS_H
 
+#ifdef __riscos__
+#include <sys/ioctl.h>
+#else
 #include <linux/ioctl.h>
+#endif
 #include "vchiq_if.h"
 
 #define VCHIQ_IOC_MAGIC 0xc4
