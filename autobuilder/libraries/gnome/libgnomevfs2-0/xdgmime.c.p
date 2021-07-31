--- libgnomevfs/xdgmime.c.orig	2012-11-22 17:37:42.478455956 +0000
+++ libgnomevfs/xdgmime.c	2012-11-22 17:40:36.733946467 +0000
@@ -60,7 +60,7 @@
 static XdgIconList *icon_list = NULL;
 static XdgIconList *generic_icon_list = NULL;

-XdgMimeCache **_caches = NULL;
+XdgMimeCache **_xdg_caches = NULL;
 static int n_caches = 0;

 const char xdg_mime_type_unknown[] = "application/octet-stream";
@@ -148,9 +148,9 @@
 	{
 	  xdg_dir_time_list_add (file_name, st.st_mtime);

-	  _caches = realloc (_caches, sizeof (XdgMimeCache *) * (n_caches + 2));
-	  _caches[n_caches] = cache;
-          _caches[n_caches + 1] = NULL;
+	  _xdg_caches = realloc (_xdg_caches, sizeof (XdgMimeCache *) * (n_caches + 2));
+	  _xdg_caches[n_caches] = cache;
+          _xdg_caches[n_caches + 1] = NULL;
 	  n_caches++;

 	  return FALSE;
@@ -466,7 +466,7 @@

   xdg_mime_init ();

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_get_mime_type_for_data (data, len, result_prio);

   mime_type = _xdg_mime_magic_lookup_data (global_magic, data, len, result_prio, NULL, 0);
@@ -501,7 +501,7 @@

   xdg_mime_init ();

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_get_mime_type_for_file (file_name, statbuf);

   base_name = _xdg_get_base_name (file_name);
@@ -563,7 +563,7 @@

   xdg_mime_init ();

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_get_mime_type_from_file_name (file_name);

   if (_xdg_glob_hash_lookup_file_name (global_hash, file_name, &mime_type, 1))
@@ -579,7 +579,7 @@
 {
   xdg_mime_init ();

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_get_mime_types_from_file_name (file_name, mime_types, n_mime_types);

   return _xdg_glob_hash_lookup_file_name (global_hash, file_name, mime_types, n_mime_types);
@@ -640,14 +640,14 @@
       generic_icon_list = NULL;
     }

-  if (_caches)
+  if (_xdg_caches)
     {
       int i;

       for (i = 0; i < n_caches; i++)
-        _xdg_mime_cache_unref (_caches[i]);
-      free (_caches);
-      _caches = NULL;
+        _xdg_mime_cache_unref (_xdg_caches[i]);
+      free (_xdg_caches);
+      _xdg_caches = NULL;
       n_caches = 0;
     }

@@ -662,7 +662,7 @@
 {
   xdg_mime_init ();

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_get_max_buffer_extents ();

   return _xdg_mime_magic_get_buffer_extents (global_magic);
@@ -673,7 +673,7 @@
 {
   const char *lookup;

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_unalias_mime_type (mime_type);

   if ((lookup = _xdg_mime_alias_list_lookup (alias_list, mime_type)) != NULL)
@@ -752,7 +752,7 @@
   const char *umime, *ubase;
   const char **parents;

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_mime_type_subclass (mime, base);

   umime = _xdg_mime_unalias_mime_type (mime);
@@ -802,7 +802,7 @@
   char **result;
   int i, n;

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_list_mime_parents (mime);

   parents = xdg_mime_get_mime_parents (mime);
@@ -905,7 +905,7 @@
 {
   xdg_mime_init ();

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_get_icon (mime);

   return _xdg_mime_icon_list_lookup (icon_list, mime);
@@ -916,7 +916,7 @@
 {
   xdg_mime_init ();

-  if (_caches)
+  if (_xdg_caches)
     return _xdg_mime_cache_get_generic_icon (mime);

   return _xdg_mime_icon_list_lookup (generic_icon_list, mime);
