--- crypto/arm_arch.h.orig	2022-12-17 12:09:46.518931898 +0000
+++ crypto/arm_arch.h	2022-12-17 12:13:00.460984510 +0000
@@ -51,6 +51,8 @@
 #    define __ARM_ARCH__ 5
 #   elif defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__)
 #    define __ARM_ARCH__ 4
+#   elif defined(__ARM_ARCH_3__)
+#    define __ARM_ARCH__ 3
 #   else
 #    error "unsupported ARM architecture"
 #   endif
