--- share/video.c.orig	2016-07-03 18:26:05.000000000 +0100
+++ share/video.c	2016-07-03 18:26:28.021285211 +0100
@@ -179,6 +179,16 @@
 
     if (window)
     {
+#ifdef __riscos__
+        /* We need a version 1.0 context, because Neverball doesn't use programmable
+	   shaders with OpenGL ES 2.
+	   Set the context profile/version after the window is created, otherwise
+	   window creation fails.  */
+        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
+        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
+        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
+#endif
+
         if ((context = SDL_GL_CreateContext(window)))
         {
             int buf, smp;
