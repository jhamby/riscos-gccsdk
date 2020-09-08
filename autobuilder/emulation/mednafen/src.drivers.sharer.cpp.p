--- src/drivers/shader.cpp.orig	2020-08-02 07:37:11.083909105 +0100
+++ src/drivers/shader.cpp	2020-08-02 07:38:02.306948700 +0100
@@ -168,7 +168,7 @@
 
  return ret;
 }
-
+#ifndef __riscos__
 void OpenGL_Blitter_Shader::SLP(GLhandleARB moe)
 {
  char buf[1000];
@@ -590,4 +590,4 @@
 {
 	Cleanup();
 }
-
+#endif 
