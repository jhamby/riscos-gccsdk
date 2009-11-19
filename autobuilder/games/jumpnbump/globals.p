--- globals.pre.orig	2009-11-13 11:26:54.243464000 +0000
+++ globals.pre	2009-11-16 13:29:07.266781800 +0000
@@ -167,6 +167,8 @@
 #define	DATA_PATH "data/jumpbump.dat"
 #elif _WIN32
 #define	DATA_PATH "data/jumpbump.dat"
+#elif __riscos__
+#define DATA_PATH "<jumpnbump$Dir>.data.jumpbump/dat"
 #else
 #define	DATA_PATH "%%PREFIX%%/share/games/jumpnbump/jumpbump.dat"
 #endif
