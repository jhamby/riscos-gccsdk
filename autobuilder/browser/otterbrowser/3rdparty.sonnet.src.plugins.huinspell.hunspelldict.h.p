--- ./3rdparty/sonnet/src/plugins/hunspell/hunspelldict.h.orig	2016-02-22 20:30:33.554624882 +0000
+++ ./3rdparty/sonnet/src/plugins/hunspell/hunspelldict.h	2016-02-22 20:30:53.366624902 +0000
@@ -28,7 +28,7 @@
 #ifdef Q_OS_MAC
 #define HUNSPELL_MAIN_DICT_PATH "/System/Library/Spelling"
 #else
-#define HUNSPELL_MAIN_DICT_PATH "/usr/share/hunspell/"
+#define HUNSPELL_MAIN_DICT_PATH "/<Hunspell$Dir>/"
 #endif
 #endif
 
