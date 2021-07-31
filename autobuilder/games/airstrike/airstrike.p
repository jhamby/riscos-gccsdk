--- src/airstrike.c.orig	2005-05-16 14:53:16.000000000 +0100
+++ src/airstrike.c	2005-05-16 14:54:09.000000000 +0100
@@ -74,7 +74,7 @@
   console_set_pos(9,254);
   console_load_bg(path_to_data("console-bg.png"));
   sprite_types_setup();
-  sprite_background_load("/usr/share/games/airstrike/bg.png","/usr/share/games/airstrike/bgmask.png");
+  sprite_background_load("<AirStrike$Dir>/data/bg.png","<AirStrike$Dir>/data/bgmask.png");
 
   level_setup();
   winds_setup();
