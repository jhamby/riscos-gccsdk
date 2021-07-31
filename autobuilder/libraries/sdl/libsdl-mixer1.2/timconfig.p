--- timidity/config.h.orig	2007-07-02 03:03:51.000000000 +0100
+++ timidity/config.h	2008-08-05 13:46:54.495000000 +0100
@@ -169,7 +169,11 @@
 
 /* You could specify a complete path, e.g. "/etc/timidity.cfg", and
    then specify the library directory in the configuration file. */
+#ifdef __riscos__
+#define CONFIG_FILE "TimidityConfig:timidity/cfg"
+#else
 #define CONFIG_FILE	"timidity.cfg"
+#endif
 #define CONFIG_FILE_ETC "/etc/timidity.cfg"
 #define CONFIG_FILE_ETC_TIMIDITY "/etc/timidity/timidity.cfg"
 
