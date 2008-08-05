--- src/u4file.cpp.orig	2008-07-23 11:18:29.618534000 +0100
+++ src/u4file.cpp	2008-07-23 11:24:58.333534000 +0100
@@ -55,56 +55,83 @@
 
 /* the possible paths where u4 for DOS can be installed */
 static const char * const paths[] = {
+#ifdef __riscos__
+    "<XU4$Dir>/ultima4/"
+#else
     "./",
     "./ultima4/",
     "/usr/lib/u4/ultima4/",
     "/usr/local/lib/u4/ultima4/"
+#endif
 };
 
 /* the possible paths where the u4 zipfiles can be installed */
 static const char * const zip_paths[] = {
+#ifdef __riscos__
+    "<XU4$Dir>/"
+#else
     "./",
     "/usr/lib/u4/",
     "/usr/local/lib/u4/"
+#endif
 };
 
 /* the possible paths where the u4 music files can be installed */
 static const char * const music_paths[] = {
+#ifdef __riscos__
+   "<XU4$Dir>/ultima4/",
+   "<XU4$Dir>/mid/"
+#else
     "./",
     "./ultima4/",
     "./mid/",
     "../mid/",
     "/usr/lib/u4/music/",
     "/usr/local/lib/u4/music/"
+#endif
 };
 
 /* the possible paths where the u4 sound files can be installed */
 static const char * const sound_paths[] = {
+#ifdef __riscos__
+   "<XU4$Dir>/ultima4/",
+   "<XU4$Dir>/sound/"
+#else
     "./",
     "./ultima4/",
     "./sound/",
     "../sound/",
     "/usr/lib/u4/sound/",
     "/usr/local/lib/u4/sound/"
+#endif
 };
 
 /* the possible paths where the u4 config files can be installed */
 static const char * const conf_paths[] = {
+#ifdef __riscos__
+   "<XU4$Dir>/conf/"
+#else
     "./",
     "./conf/",
     "../conf/",
     "/usr/lib/u4/",
     "/usr/local/lib/u4/"
+#endif
 };
 
 /* the possible paths where the u4 graphics files can be installed */
 static const char * const graphics_paths[] = {
+#ifdef __riscos__
+   "<XU4$Dir>/ultima4/",
+   "<XU4$Dir>/graphics/",
+#else
     "./",
     "./ultima4/",
     "./graphics/",
     "../graphics/",
     "/usr/lib/u4/graphics/",
     "/usr/local/lib/u4/graphics/"
+#endif
 };
 
 #ifdef MACOSX
