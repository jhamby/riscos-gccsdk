--- Source_Files/RenderMain/OGL_Setup.h.orig	2009-10-05 20:43:29.000000000 +0100
+++ Source_Files/RenderMain/OGL_Setup.h	2009-12-11 11:17:58.959442700 +0000
@@ -118,6 +118,7 @@
 	}
 }
 
+#ifdef HAVE_OPENGL
 void SglColor3f(GLfloat r, GLfloat g, GLfloat b);
 void SglColor3fv(const GLfloat* v);
 void SglColor3ub(GLubyte r, GLubyte g, GLubyte b);
@@ -127,6 +128,7 @@
 void SglColor4fv(const GLfloat* v);
 void SglColor4fva(const GLfloat* v);
 void SglColor4usv(const GLushort* v);
+#endif
 
 // Initializer; returns whether or not OpenGL is present
 bool OGL_Initialize();
