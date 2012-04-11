--- port.h.orig	2012-04-11 08:34:00.000000000 +0100
+++ port.h	2012-04-11 08:35:10.000000000 +0100
@@ -331,6 +331,10 @@
 #define TITLE "Snes9x"
 #endif

+#ifdef __riscos__
+#define SYS_CONFIG_FILE "/<Snes9x$Dir>/snes9x.conf"
+#endif
+
 #if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__x86_64__) || defined(__alpha__) || defined(__MIPSEL__) || defined(_M_IX86) || defined(_M_X64) || defined(__arm__)
 #define LSB_FIRST
 #define FAST_LSB_WORD_ACCESS
