--- src/GUI/mapview.cpp.orig	2021-08-01 08:45:57.032321941 +0100
+++ src/GUI/mapview.cpp	2021-08-01 08:46:29.135706366 +0100
@@ -1017,10 +1017,11 @@
 void MapView::useOpenGL(bool use)
 {
 	_opengl = use;
-
+#ifndef __riscos__
 	if (use)
 		setViewport(new OPENGL_WIDGET);
 	else
+#endif
 		setViewport(new QWidget);
 }
 
