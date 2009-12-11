--- Source_Files/RenderMain/OGL_Setup.cpp.orig	2009-08-16 23:26:15.000000000 +0100
+++ Source_Files/RenderMain/OGL_Setup.cpp	2009-12-11 11:18:31.746442700 +0000
@@ -592,6 +592,8 @@
 }
 
 /* These don't belong here */
+#ifdef HAVE_OPENGL
+
 void SglColor3f(GLfloat r, GLfloat g, GLfloat b) {
   GLfloat ov[3] = {sRGB_frob(r), sRGB_frob(g), sRGB_frob(b)};
   glColor3fv(ov);
@@ -636,3 +638,5 @@
   GLfloat ov[4] = {sRGB_frob(iv[0]*(1.f/65535.f)), sRGB_frob(iv[1]*(1.f/65535.f)), sRGB_frob(iv[2]*(1.f/65535.f)), iv[3]};
   glColor4fv(ov);
 }
+
+#endif
