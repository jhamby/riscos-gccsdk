--- subversion/libsvn_client/export.c.orig	2009-01-26 12:30:42.000000000 +0100
+++ subversion/libsvn_client/export.c	2009-08-11 23:09:12.000000000 +0200
@@ -101,7 +101,7 @@
   apr_hash_t *kw = NULL;
   svn_subst_eol_style_t style;
   apr_hash_t *props;
-  svn_string_t *eol_style, *keywords, *executable, *special;
+  svn_string_t *eol_style, *keywords, *executable, *filetype, *special;
   const char *eol = NULL;
   svn_boolean_t local_mod = FALSE;
   apr_time_t tm;
@@ -162,6 +162,8 @@
                           APR_HASH_KEY_STRING);
   executable = apr_hash_get(props, SVN_PROP_EXECUTABLE,
                             APR_HASH_KEY_STRING);
+  filetype = apr_hash_get (props, SVN_PROP_FILETYPE,
+                           APR_HASH_KEY_STRING);
 
   if (eol_style)
     SVN_ERR(get_eol_style(&style, &eol, eol_style->data, native_eol));
@@ -225,6 +227,9 @@
   if (!err && executable)
     err = svn_io_set_file_executable(dst_tmp, TRUE, FALSE, pool);
 
+  if (!err && filetype)
+    err = svn_io_set_file_filetype (dst_tmp, filetype, pool);
+
   if (!err)
     err = svn_io_set_file_affected_time(tm, dst_tmp, pool);
 
@@ -504,6 +509,7 @@
   const svn_string_t *eol_style_val;
   const svn_string_t *keywords_val;
   const svn_string_t *executable_val;
+  const svn_string_t *filetype_val;
   svn_boolean_t special;
 
   /* Any keyword vals to be substituted */
@@ -706,6 +712,9 @@
   else if (strcmp(name, SVN_PROP_EXECUTABLE) == 0)
     fb->executable_val = svn_string_dup(value, fb->pool);
 
+  else if (strcmp (name, SVN_PROP_FILETYPE) == 0)
+    fb->filetype_val = svn_string_dup (value, fb->pool);
+
   /* Try to fill out the baton's keywords-structure too. */
   else if (strcmp(name, SVN_PROP_ENTRY_COMMITTED_REV) == 0)
     fb->revision = apr_pstrdup(fb->pool, value->data);
@@ -806,6 +815,9 @@
 
   if (fb->executable_val)
     SVN_ERR(svn_io_set_file_executable(fb->path, TRUE, FALSE, pool));
+ 
+  if (fb->filetype_val)
+    SVN_ERR (svn_io_set_file_filetype(fb->path, fb->filetype_val, pool));
 
   if (fb->date && (! fb->special))
     SVN_ERR(svn_io_set_file_affected_time(fb->date, fb->path, pool));
