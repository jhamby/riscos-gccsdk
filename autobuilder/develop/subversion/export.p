--- subversion/libsvn_client/export.c.orig	2008-04-07 02:22:45.000000000 -0700
+++ subversion/libsvn_client/export.c	2008-12-28 08:27:11.000000000 -0800
@@ -103,7 +103,7 @@
   svn_subst_eol_style_t style;
   apr_hash_t *props;
   const char *base;
-  svn_string_t *eol_style, *keywords, *executable, *externals, *special;
+          svn_string_t *eol_style, *keywords, *filetype, *executable, *externals, *special;
   const char *eol = NULL;
   svn_boolean_t local_mod = FALSE;
   apr_time_t tm;
@@ -149,6 +149,8 @@
                           APR_HASH_KEY_STRING);
   executable = apr_hash_get(props, SVN_PROP_EXECUTABLE,
                             APR_HASH_KEY_STRING);
+  filetype = apr_hash_get (props, SVN_PROP_FILETYPE,
+                           APR_HASH_KEY_STRING);
   externals = apr_hash_get(props, SVN_PROP_EXTERNALS,
                            APR_HASH_KEY_STRING);
   special = apr_hash_get(props, SVN_PROP_SPECIAL,
@@ -201,6 +203,8 @@
   if (executable)
     SVN_ERR(svn_io_set_file_executable(to, TRUE,
                                        FALSE, pool));
+  if (filetype)
+    SVN_ERR (svn_io_set_file_filetype (to, filetype, pool));
 
   if (! special)
     SVN_ERR(svn_io_set_file_affected_time(tm, to, pool));
@@ -469,6 +473,7 @@
   const svn_string_t *eol_style_val;
   const svn_string_t *keywords_val;
   const svn_string_t *executable_val;
+  const svn_string_t *filetype_val;
   svn_boolean_t special;
 
   /* Any keyword vals to be substituted */
@@ -665,6 +670,9 @@
   else if (strcmp(name, SVN_PROP_EXECUTABLE) == 0)
     fb->executable_val = svn_string_dup(value, fb->pool);
 
+  else if (strcmp (name, SVN_PROP_FILETYPE) == 0)
+    fb->filetype_val = svn_string_dup (value, fb->pool);
+
   /* Try to fill out the baton's keywords-structure too. */
   else if (strcmp(name, SVN_PROP_ENTRY_COMMITTED_REV) == 0)
     fb->revision = apr_pstrdup(fb->pool, value->data);
@@ -762,6 +770,9 @@
   if (fb->executable_val)
     SVN_ERR(svn_io_set_file_executable(fb->path, TRUE, FALSE, pool));
 
+  if (fb->filetype_val)
+    SVN_ERR (svn_io_set_file_filetype(fb->path, fb->filetype_val, pool));
+
   if (fb->date && (! fb->special))
     SVN_ERR(svn_io_set_file_affected_time(fb->date, fb->path, pool));
 
