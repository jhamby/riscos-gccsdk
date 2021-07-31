--- mono/metadata/mono-config.c.orig	2011-08-13 18:06:22.000000000 +0100
+++ mono/metadata/mono-config.c	2011-08-13 18:06:27.000000000 +0100
@@ -41,6 +41,8 @@
 #define CONFIG_OS "hpux"
 #elif defined(__HAIKU__)
 #define CONFIG_OS "haiku"
+#elif defined(__riscos__)
+#define CONFIG_OS "RISC OS"
 #else
 #warning Unknown operating system
 #define CONFIG_OS "unknownOS"
