--- poppler/CairoFontEngine.cc.orig	2009-08-01 15:28:37.000000000 -0700
+++ poppler/CairoFontEngine.cc	2009-08-01 15:29:29.000000000 -0700
@@ -149,7 +149,7 @@
   else
     _ft_open_faces = data->next;
 
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
   munmap ((char*)data->bytes, data->size);
 #else
   munmap (data->bytes, data->size);
@@ -195,7 +195,7 @@
 
   for (l = _ft_open_faces; l; l = l->next) {
     if (_ft_face_data_equal (l, &tmpl)) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
       munmap ((char*)tmpl.bytes, tmpl.size);
 #else
       munmap (tmpl.bytes, tmpl.size);
@@ -209,7 +209,7 @@
 
   /* not a dup, open and insert into list */
   if (FT_New_Face (lib, filename, 0, &tmpl.face)) {
-#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4)
+#if defined(__SUNPRO_CC) && defined(__sun) && defined(__SVR4) || defined(__riscos__)
     munmap ((char*)tmpl.bytes, tmpl.size);
 #else
     munmap (tmpl.bytes, tmpl.size);
