--- poppler/CairoFontEngine.cc.orig	2016-05-01 23:08:03.000000000 +0100
+++ poppler/CairoFontEngine.cc	2016-06-05 12:24:58.098030882 +0100
@@ -259,7 +259,7 @@
     _ft_open_faces = data->next;
 
   if (data->fd != -1) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
     munmap ((char*)data->bytes, data->size);
 #else
     munmap (data->bytes, data->size);
@@ -318,7 +318,7 @@
   for (l = _ft_open_faces; l; l = l->next) {
     if (_ft_face_data_equal (l, &tmpl)) {
       if (tmpl.fd != -1) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
         munmap ((char*)tmpl.bytes, tmpl.size);
 #else
         munmap (tmpl.bytes, tmpl.size);
@@ -339,7 +339,7 @@
 			  0, &tmpl.face))
   {
     if (tmpl.fd != -1) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
       munmap ((char*)tmpl.bytes, tmpl.size);
 #else
       munmap (tmpl.bytes, tmpl.size);
