--- src/GUI/graphview.cpp.orig	2021-08-01 08:43:19.651503910 +0100
+++ src/GUI/graphview.cpp	2021-08-01 08:43:59.475009835 +0100
@@ -536,9 +536,11 @@
 
 void GraphView::useOpenGL(bool use)
 {
+#ifndef __riscos__
 	if (use)
 		setViewport(new OPENGL_WIDGET);
 	else
+#endif
 		setViewport(new QWidget);
 }
 
