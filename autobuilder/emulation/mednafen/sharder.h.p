--- src/drivers/shader.h.orig	2017-03-05 13:31:47.923085888 +0000
+++ src/drivers/shader.h	2017-03-05 13:32:12.830590041 +0000
@@ -15,6 +15,7 @@
         SHADER_IPYNOTXSHARPER,
 };
 
+#ifndef __riscos__
 class OpenGL_Blitter;
 class OpenGL_Blitter_Shader
 {
@@ -45,5 +46,5 @@
  CompiledShader CSP[CSP_COUNT];
  OpenGL_Blitter* const oblt;
 };
-
+#endif
 #endif
