--- gamecycle.cpp.orig	2021-10-09 17:15:01.542738437 +0100
+++ gamecycle.cpp	2021-10-09 17:16:05.918251903 +0100
@@ -337,9 +337,9 @@
 
 				if (SUBSTATE==0) {
 					init_paths();
-					if (!cargar_configuracion("MoG.cfg")) {
+					if (!cargar_configuracion("/<MazeOfGalious$Dir>/Mog.cfg")) {
 						configuracion_por_defecto();
-						guardar_configuracion("MoG.cfg");
+						guardar_configuracion("/<MazeOfGalious$Dir>/Mog.cfg");
 					} /* if */ 
 					GameEnd();
 					GameInit(dx,dy);
@@ -1377,7 +1377,7 @@
 					 (keyboard[SDLK_k] && !old_keyboard[SDLK_k]))) {
 					STATE=2;
 					SUBSTATE=0;
-					guardar_configuracion("MoG.cfg");
+					guardar_configuracion("/<MazeOfGalious$Dir>/Mog.cfg");
 				} /* if */ 
 			}
 			break;
@@ -2277,7 +2277,7 @@
 
 				if (keyboard[SDLK_ESCAPE] && !old_keyboard[SDLK_ESCAPE]) {
 					STATE=OLDSTATE;
-					guardar_configuracion("MoG.cfg");
+					guardar_configuracion("/<MazeOfGalious$Dir>/Mog.cfg");
 				} /* if */ 
 				if (keyboard[DOWN_KEY] && !old_keyboard[DOWN_KEY] && MENUOPTION<2) MENUOPTION++;
 				if (keyboard[UP_KEY] && !old_keyboard[UP_KEY] && MENUOPTION>0) MENUOPTION--;
@@ -2286,7 +2286,7 @@
 					Sound_release_music();
 					STATE=0;
 					SUBSTATE=0;
-					guardar_configuracion("MoG.cfg");
+					guardar_configuracion("/<MazeOfGalious$Dir>/Mog.cfg");
 				} /* if */ 
 
 				if (keyboard[RIGHT_KEY] && MENUOPTION==1 && music_volume<127) {
