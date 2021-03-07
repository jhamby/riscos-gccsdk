--- win/gl/gl_font.c.orig	2021-03-07 20:31:43.758527582 +0000
+++ win/gl/gl_font.c	2021-03-07 20:33:37.803894610 +0000
@@ -47,25 +47,25 @@
   {
     case 8:
       if (! sdlgl_font_8)
-        sdlgl_font_8 = sdlgl_load_tileset("/usr/share/pixmaps/glhack/glfont8.png", 8,8, 1,0,
+        sdlgl_font_8 = sdlgl_load_tileset("/<GLHack$Dir>/usr/glfont8.png", 8,8, 1,0,
             NULL,NULL);
       return sdlgl_font_8;
 
     case 14:
       if (! sdlgl_font_14)
-        sdlgl_font_14 = sdlgl_load_tileset("/usr/share/pixmaps/glhack/glfont14.png", 8,14, 1,0,
+        sdlgl_font_14 = sdlgl_load_tileset("/<GLHack$Dir>/usr/glfont14.png", 8,14, 1,0,
             NULL,NULL);
       return sdlgl_font_14;
 
     case 20:
       if (! sdlgl_font_20)
-        sdlgl_font_20 = sdlgl_load_tileset("/usr/share/pixmaps/glhack/glfont20.png", 10,20, 1,0,
+        sdlgl_font_20 = sdlgl_load_tileset("/<GLHack$Dir>/usr/glfont20.png", 10,20, 1,0,
             NULL,NULL);
       return sdlgl_font_20;
 
     case 22:
       if (! sdlgl_font_22)
-        sdlgl_font_22 = sdlgl_load_tileset("/usr/share/pixmaps/glhack/glfont22.png", 12,22, 1,0,
+        sdlgl_font_22 = sdlgl_load_tileset("/<GLHack$Dir>/usr/glfont22.png", 12,22, 1,0,
             NULL,NULL);
       return sdlgl_font_22;
 
