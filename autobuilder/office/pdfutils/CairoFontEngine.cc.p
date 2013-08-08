--- poppler/CairoFontEngine.cc.orig	2013-05-23 17:02:06.241881294 +0100
+++ poppler/CairoFontEngine.cc	2013-05-23 17:03:11.825704843 +0100
@@ -256,7 +256,7 @@
   else
     _ft_open_faces = data->next;

-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
   munmap ((char*)data->bytes, data->size);
 #else
   munmap (data->bytes, data->size);
@@ -312,7 +312,7 @@
   for (l = _ft_open_faces; l; l = l->next) {
     if (_ft_face_data_equal (l, &tmpl)) {
       if (tmpl.fd != -1) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
         munmap ((char*)tmpl.bytes, tmpl.size);
 #else
         munmap (tmpl.bytes, tmpl.size);
@@ -331,7 +331,7 @@
 			  0, &tmpl.face))
   {
     if (tmpl.fd != -1) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
       munmap ((char*)tmpl.bytes, tmpl.size);
 #else
       munmap (tmpl.bytes, tmpl.size);
