--- share/solid_draw.c.orig	2015-08-22 17:26:12.000000000 +0100
+++ share/solid_draw.c	2015-08-22 17:26:38.632015389 +0100
@@ -996,8 +996,8 @@
             glEnable (GL_POINT_SPRITE);
             glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
             glPointParameterfv_(GL_POINT_DISTANCE_ATTENUATION, c);
-            glPointParameterf_ (GL_POINT_SIZE_MIN, 1);
-            glPointParameterf_ (GL_POINT_SIZE_MAX, s);
+            glPointParameterf (GL_POINT_SIZE_MIN, 1);
+            glPointParameterf (GL_POINT_SIZE_MAX, s);
         }
         else
         {
