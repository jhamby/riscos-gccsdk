--- ./src/engine/system/sdl_system.cpp.orig	2011-09-08 13:55:19.466974900 +0100
+++ ./src/engine/system/sdl_system.cpp	2011-09-08 14:20:51.523687800 +0100
@@ -21,6 +21,10 @@
 #include "engine/display/display.hpp"
 #include "math/size.hpp"
 
+#ifdef __riscos__
+#include "config.h"
+#endif
+
 SDLSystem::SDLSystem()
 {
   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) 
