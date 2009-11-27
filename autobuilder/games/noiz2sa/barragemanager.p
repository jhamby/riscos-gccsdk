--- src/barragemanager.cc.orig	2009-11-13 13:44:18.401464000 +0000
+++ src/barragemanager.cc	2009-11-13 13:44:59.447464000 +0000
@@ -27,7 +27,7 @@
 #include "foe.h"
 
 #define BARRAGE_PATTERN_MAX 32
-#define SHARE_LOC "/usr/share/games/noiz2sa/"
+#define SHARE_LOC "/<Noiz2sa$Dir>/data/"
 
 static Barrage barragePattern[BARRAGE_TYPE_NUM][BARRAGE_PATTERN_MAX];
 static Barrage *barrageQueue[BARRAGE_TYPE_NUM][BARRAGE_PATTERN_MAX];
