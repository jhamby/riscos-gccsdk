--- main.cpp.orig	2021-10-15 09:59:31.925702405 +0100
+++ main.cpp	2021-10-15 10:02:07.041244655 +0100
@@ -44,8 +44,8 @@
 int init_time=0;
 
 /* Paths: */ 
-char *default_g_path="graphics/alternate/";
-char *default_s_path="sound/original/";
+char *default_g_path="/<MazeOfGalious$Dir>/graphics/alternate/";
+char *default_s_path="/<MazeOfGalious$Dir>/sound/original/";
 char **g_paths=0;
 int n_g_paths=0,act_g_path=0;
 char *g_path=0;
@@ -54,7 +54,7 @@
 char *s_path=0;
 
 char *confdir;
-char *datadir="/usr/share/games/mazeofgalious/";
+char *datadir="/<MazeOfGalious$Dir>/";
 
 extern int music_volume,sfx_volume;
 extern int fighting_demon;
@@ -185,7 +185,7 @@
 						ReleaseGraphics();
 						ReloadGraphics(SCREEN_X,SCREEN_Y);
 						chdir(confdir);
-						guardar_configuracion("MoG.cfg");
+						guardar_configuracion("/<MazeOfGalious$Dir>/MoG.cfg");
 						chdir(datadir);
 						if (fighting_demon!=0) redo_demonintro(fighting_demon,0,SCREEN_X,SCREEN_Y);
 					} /* if */ 
@@ -198,7 +198,7 @@
 						ReloadSound();
 						music_recovery();
 						chdir(confdir);
-						guardar_configuracion("MoG.cfg");
+						guardar_configuracion("/<MazeOfGalious$Dir>/MoG.cfg");
 						chdir(datadir);
 						Mix_VolumeMusic(music_volume);
 						SetSFXVolume(sfx_volume);
