--- src/global.cc.orig	2010-08-21 12:19:51.000000000 +0100
+++ src/global.cc	2010-08-19 23:36:53.000000000 +0100
@@ -370,13 +370,13 @@
   surfaceDB.loadSurface(FN_FONT_NUMBERS_RIGHT);
   surfaceDB.loadSurface(FN_EXPLOSION_NORMAL);
   surfaceDB.loadSurface(FN_EXPLOSION_ENEMY);
-  surfaceDB.loadSurface("/usr/share/games/alienblaster/images/bannerExcellent.bmp", true);
-  surfaceDB.loadSurface("/usr/share/games/alienblaster/images/bannerYouRule.bmp", true);
-  surfaceDB.loadSurface("/usr/share/games/alienblaster/images/bannerHeiho.bmp", true);
-  surfaceDB.loadSurface("/usr/share/games/alienblaster/images/bannerHealth.bmp", true);
-  surfaceDB.loadSurface("/usr/share/games/alienblaster/images/bannerEnemysKilled.bmp", true);
-  surfaceDB.loadSurface("/usr/share/games/alienblaster/images/bannerBonus100.bmp", true);
-  surfaceDB.loadSurface("/usr/share/games/alienblaster/images/bannerBonus200.bmp", true);
+  surfaceDB.loadSurface("/<AlienBlaster$Dir>/images/bannerExcellent.bmp", true);
+  surfaceDB.loadSurface("/<AlienBlaster$Dir>/images/bannerYouRule.bmp", true);
+  surfaceDB.loadSurface("/<AlienBlaster$Dir>/images/bannerHeiho.bmp", true);
+  surfaceDB.loadSurface("/<AlienBlaster$Dir>/images/bannerHealth.bmp", true);
+  surfaceDB.loadSurface("/<AlienBlaster$Dir>/images/bannerEnemysKilled.bmp", true);
+  surfaceDB.loadSurface("/<AlienBlaster$Dir>/images/bannerBonus100.bmp", true);
+  surfaceDB.loadSurface("/<AlienBlaster$Dir>/images/bannerBonus200.bmp", true);
 }
 
 
