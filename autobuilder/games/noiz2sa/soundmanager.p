--- src/soundmanager.c.orig	2010-02-18 00:12:27.000000000 +0000
+++ src/soundmanager.c	2010-02-18 00:13:01.000000000 +0000
@@ -21,7 +21,7 @@
 static int useAudio = 0;

 #define MUSIC_NUM 7
-#define SHARE_LOC "/usr/share/games/noiz2sa/"
+#define SHARE_LOC "/<Noiz2sa$Dir>/data/"

 static char *musicFileName[MUSIC_NUM] = {
   "stg0.ogg", "stg1.ogg", "stg2.ogg", "stg3.ogg", "stg4.ogg", "stg5.ogg", "stg00.ogg",
