--- src/gui/sdl_mapper.cpp.orig	2010-05-10 19:58:06.000000000 +0100
+++ src/gui/sdl_mapper.cpp	2010-11-18 23:46:16.000000000 +0000
@@ -2397,7 +2397,13 @@
 
 	usescancodes = false;
 
+#ifdef __riscos__
+	// Can't use scan code for RISC OS
+	if (0)
+	{
+#else
 	if (section->Get_bool("usescancodes")) {
+#endif
 		usescancodes=true;
 
 		/* Note: table has to be tested/updated for various OSs */
@@ -2422,7 +2428,7 @@
 		sdlkey_map[0x5E]=SDLK_RALT;
 		sdlkey_map[0x40]=SDLK_KP5;
 		sdlkey_map[0x41]=SDLK_KP6;
-#elif !defined (WIN32) /* => Linux & BSDs */
+#elif !defined (WIN32) /* => Linux & BSDs */
 		bool evdev_input = false;
-#ifdef C_X11_XKB
+#if defined (C_X11_XKB) && !defined (__riscos__)
 		SDL_SysWMinfo info;
