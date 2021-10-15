--- gamecycle.cpp.orig	2021-10-15 10:07:00.696405487 +0100
+++ gamecycle.cpp	2021-10-15 10:08:16.152193961 +0100
@@ -341,9 +341,9 @@
 				if (SUBSTATE==0) {
 					init_paths();
 					chdir(confdir);
-					if (!cargar_configuracion("MoG.cfg")) {
+					if (!cargar_configuracion("/<MazeOfGalious$Dir>/MoG.cfg")) {
 						configuracion_por_defecto();
-						guardar_configuracion("MoG.cfg");
+						guardar_configuracion("/<MazeOfGalious$Dir>/MoG.cfg");
 					} /* if */ 
 					chdir(datadir);
 					GameEnd();
@@ -1383,7 +1383,7 @@
 					STATE=2;
 					SUBSTATE=0;
 					chdir(confdir);
-					guardar_configuracion("MoG.cfg");
+					guardar_configuracion("/<MazeOfGalious$Dir>/MoG.cfg");
 					chdir(datadir);
 				} /* if */ 
 			}
@@ -2288,7 +2288,7 @@
 				if (keyboard[SDLK_ESCAPE] && !old_keyboard[SDLK_ESCAPE]) {
 					STATE=OLDSTATE;
 					chdir(confdir);
-					guardar_configuracion("MoG.cfg");
+					guardar_configuracion("/<MazeOfGalious$Dir>/MoG.cfg");
 					chdir(datadir);
 				} /* if */ 
 				if (keyboard[DOWN_KEY] && !old_keyboard[DOWN_KEY] && MENUOPTION<2) MENUOPTION++;
@@ -2299,7 +2299,7 @@
 					STATE=0;
 					SUBSTATE=0;
 					chdir(confdir);
-					guardar_configuracion("MoG.cfg");
+					guardar_configuracion("/<MazeOfGalious$Dir>/MoG.cfg");
 					chdir(datadir);
 				} /* if */ 
 
