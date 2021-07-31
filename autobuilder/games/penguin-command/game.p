--- src/game.c.orig	2007-08-09 13:17:44.253518100 +0100
+++ src/game.c	2007-08-09 13:18:47.483518100 +0100
@@ -157,7 +157,9 @@
     float deg;
 
     SDL_GetMouseState(&MouseX, &MouseY);
-    deg=atan((float)(MouseY-(y+20))/(float)(MouseX-x));
+    if (MouseX == x) deg = -1.5707;
+    else deg=atan((float)(MouseY-(y+20))/(float)(MouseX-x));
+
     if (MouseX<x) {
 	add+=20;
     }
