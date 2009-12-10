--- poppler/CairoFontEngine.cc.orig	2009-09-09 14:22:31.000000000 -0700
+++ poppler/CairoFontEngine.cc	2009-12-10 09:06:25.000000000 -0800
@@ -242,7 +242,7 @@
   else
     _ft_open_faces = data->next;
 
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
   munmap ((char*)data->bytes, data->size);
 #else
   munmap (data->bytes, data->size);
@@ -288,7 +288,7 @@
 
   for (l = _ft_open_faces; l; l = l->next) {
     if (_ft_face_data_equal (l, &tmpl)) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
       munmap ((char*)tmpl.bytes, tmpl.size);
 #else
       munmap (tmpl.bytes, tmpl.size);
@@ -305,7 +305,7 @@
 			  (FT_Byte *) tmpl.bytes, tmpl.size,
 			  0, &tmpl.face))
   {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
     munmap ((char*)tmpl.bytes, tmpl.size);
 #else
     munmap (tmpl.bytes, tmpl.size);
