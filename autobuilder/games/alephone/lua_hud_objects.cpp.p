--- Source_Files/Lua/lua_hud_objects.cpp.orig	2009-10-05 20:43:28.000000000 +0100
+++ Source_Files/Lua/lua_hud_objects.cpp	2009-12-11 11:09:54.105442700 +0000
@@ -387,7 +387,11 @@
         int resource_id = lua_tointeger(L, -1);
 
         // blitter from image
+#ifdef HAVE_OPENGL
         Image_Blitter *blitter = (get_screen_mode()->acceleration == _opengl_acceleration) ? new OGL_Blitter() : new Image_Blitter();
+#else
+        Image_Blitter *blitter = new Image_Blitter();
+#endif
         if (!blitter->Load(resource_id))
         {
             lua_pushnil(L);
@@ -447,7 +451,11 @@
 		image.LoadFromFile(File, ImageLoader_Opacity, 0);
 	
 	// blitter from image
-	Image_Blitter *blitter = (get_screen_mode()->acceleration == _opengl_acceleration) ? new OGL_Blitter() : new Image_Blitter();
+#ifdef HAVE_OPENGL
+        Image_Blitter *blitter = (get_screen_mode()->acceleration == _opengl_acceleration) ? new OGL_Blitter() : new Image_Blitter();
+#else
+        Image_Blitter *blitter = new Image_Blitter();
+#endif
 	if (!blitter->Load(image))
 	{
 		lua_pushnil(L);
@@ -851,8 +859,10 @@
 	
 	FontSpecifier *ff = new FontSpecifier(f);
 	ff->Init();
+#ifdef HAVE_OPENGL
 	if (alephone::Screen::instance()->openGL())
 		ff->OGL_Reset(true);
+#endif
 	if (ff->LineSpacing <= 0)
 	{
 		lua_pushnil(L);
