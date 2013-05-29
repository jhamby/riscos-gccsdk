--- poppler/CairoFontEngine.cc.orig	2013-05-23 17:58:15.044736180 +0100
+++ poppler/CairoFontEngine.cc	2013-05-23 17:58:54.716634922 +0100
@@ -251,7 +251,7 @@
   else
     _ft_open_faces = data->next;

-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
   munmap ((char*)data->bytes, data->size);
 #else
   munmap (data->bytes, data->size);
@@ -307,7 +307,7 @@
   for (l = _ft_open_faces; l; l = l->next) {
     if (_ft_face_data_equal (l, &tmpl)) {
       if (tmpl.fd != -1) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
         munmap ((char*)tmpl.bytes, tmpl.size);
 #else
         munmap (tmpl.bytes, tmpl.size);
@@ -326,7 +326,7 @@
 			  0, &tmpl.face))
   {
     if (tmpl.fd != -1) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
       munmap ((char*)tmpl.bytes, tmpl.size);
 #else
       munmap (tmpl.bytes, tmpl.size);
