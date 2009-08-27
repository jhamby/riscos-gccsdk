--- subversion/libsvn_wc/translate.c.orig	2009-02-16 07:25:05.000000000 +0100
+++ subversion/libsvn_wc/translate.c	2009-08-12 00:08:55.000000000 +0200
@@ -368,3 +368,26 @@
 
   return SVN_NO_ERROR;
 }
+
+svn_error_t *
+svn_wc__maybe_set_filetype (svn_boolean_t *did_set,
+                              const char *path,
+                              svn_wc_adm_access_t *adm_access,
+                              apr_pool_t *pool)
+{
+  const svn_string_t *propval;
+  SVN_ERR (svn_wc_prop_get (&propval, SVN_PROP_FILETYPE, path, adm_access,
+                            pool));
+
+  if (propval != NULL)
+    {
+      SVN_ERR (svn_io_set_file_filetype (path, propval, pool));
+      if (did_set)
+        *did_set = TRUE;
+    }
+  else if (did_set)
+    *did_set = FALSE;
+
+  return SVN_NO_ERROR;
+}
+
