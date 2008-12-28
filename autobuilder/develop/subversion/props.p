--- subversion/libsvn_wc/props.c.orig	2008-07-14 10:13:27.000000000 -0700
+++ subversion/libsvn_wc/props.c	2008-12-28 09:07:29.000000000 -0800
@@ -2292,6 +2292,12 @@
     {
       return wcprop_get(value, name, path, adm_access, pool);
     }
+
+  if (kind == svn_node_file && strcmp (name, SVN_PROP_FILETYPE) == 0)
+    {
+      SVN_ERR(svn_io_set_file_filetype (path, value, pool));
+    }
+
   if (kind == svn_prop_entry_kind)
     {
       return svn_error_createf   /* we don't do entry properties here */
@@ -2324,6 +2330,7 @@
                                   SVN_PROP_EXTERNALS,
                                   NULL };
   const char *dir_prohibit[] = { SVN_PROP_EXECUTABLE,
+                                 SVN_PROP_FILETYPE,
                                  SVN_PROP_KEYWORDS,
                                  SVN_PROP_EOL_STYLE,
                                  SVN_PROP_MIME_TYPE,
