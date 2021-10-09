--- main.cpp.orig	2021-03-06 17:21:37.151008541 +0000
+++ main.cpp	2021-03-06 17:25:53.324720586 +0000
@@ -40,8 +40,8 @@
 int init_time=0;
 
 /* Paths: */ 
-char *default_g_path="graphics/original/";
-char *default_s_path="sound/original/";
+char *default_g_path="/<MazeOfGalious$Dir>/graphics/original/";
+char *default_s_path="/<MazeOfGalious$Dir>/sound/original/";
 char **g_paths=0;
 int n_g_paths=0,act_g_path=0;
 char *g_path=0;
@@ -165,7 +165,7 @@
 						g_path=g_paths[act_g_path];
 						ReleaseGraphics();
 						ReloadGraphics(SCREEN_X,SCREEN_Y);
-						guardar_configuracion("MoG.cfg");
+						guardar_configuracion("/<MazeOfGalious$Dir>/MoG.cfg");
 						if (fighting_demon!=0) redo_demonintro(fighting_demon,0,SCREEN_X,SCREEN_Y);
 					} /* if */ 
 					if (event.key.keysym.sym==SDLK_F11) {
@@ -176,7 +176,7 @@
 						ReleaseSound(false);
 						ReloadSound();
 						music_recovery();
-						guardar_configuracion("MoG.cfg");
+						guardar_configuracion("<MazeOfGalious$Dir>/MoG.cfg");
 						Mix_VolumeMusic(music_volume);
 						SetSFXVolume(sfx_volume);
 					} /* if */ 
