--- win/gl/gl_tile.c.orig	2021-03-07 20:31:30.094848363 +0000
+++ win/gl/gl_tile.c	2021-03-07 20:34:32.346660317 +0000
@@ -134,11 +134,11 @@
    * removed and freed.
    */
   if (iflags.wc_tile_height == 16)
-    sdlgl_tiles = sdlgl_load_tileset("/usr/share/pixmaps/glhack/gltile16.png", 16,16, 0,0, NULL,NULL);
+    sdlgl_tiles = sdlgl_load_tileset("/<GLHack$Dir>/usr/gltile16.png", 16,16, 0,0, NULL,NULL);
   else if (iflags.wc_tile_height == 64)
-    sdlgl_tiles = sdlgl_load_tileset("/usr/share/pixmaps/glhack/gltile64.png", 48,64, 0,0, NULL,NULL);
+    sdlgl_tiles = sdlgl_load_tileset("/<GLHack$Dir>/usr/gltile64.png", 48,64, 0,0, NULL,NULL);
   else
-    sdlgl_tiles = sdlgl_load_tileset("/usr/share/pixmaps/glhack/gltile32.png", 32,32, 0,0, NULL,NULL);
+    sdlgl_tiles = sdlgl_load_tileset("/<GLHack$Dir>/usr/gltile32.png", 32,32, 0,0, NULL,NULL);
 
   assert(sdlgl_tiles);
 }
@@ -152,7 +152,7 @@
   int x, y;
   int w1, h1, w2, h2;
     
-  logo_set = sdlgl_load_tileset("/usr/share/pixmaps/glhack/gllogo.png", 16,16, 0,0, &logo_w, &logo_h);
+  logo_set = sdlgl_load_tileset("/<GLHack$Dir>/usr/gllogo.png", 16,16, 0,0, &logo_w, &logo_h);
 
   assert(logo_set);
 
@@ -308,7 +308,7 @@
     do_load_tileset();
   }
 
-  rip_set = sdlgl_load_tileset("/usr/share/pixmaps/glhack/glrip.png", 30,30, 0,1, &rip_w, &rip_h);
+  rip_set = sdlgl_load_tileset("/<GLHack$Dir>/usr/glrip.png", 30,30, 0,1, &rip_w, &rip_h);
 
   assert(rip_set);
 
