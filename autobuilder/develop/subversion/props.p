--- subversion/libsvn_wc/props.c.orig	2009-04-07 14:48:12.000000000 +0200
+++ subversion/libsvn_wc/props.c	2009-08-13 20:00:15.000000000 +0200
@@ -2253,6 +2253,7 @@
                                   SVN_PROP_EXTERNALS,
                                   NULL };
   const char *dir_prohibit[] = { SVN_PROP_EXECUTABLE,
+                                 SVN_PROP_FILETYPE,
                                  SVN_PROP_KEYWORDS,
                                  SVN_PROP_EOL_STYLE,
                                  SVN_PROP_MIME_TYPE,
@@ -2438,6 +2439,11 @@
         SVN_ERR(svn_io_set_file_executable(path, TRUE, TRUE, pool));
     }
 
+  if (entry->kind == svn_node_file && strcmp (name, SVN_PROP_FILETYPE) == 0)
+    {
+        SVN_ERR (svn_io_set_file_filetype (path, value, pool));
+    }
+
   if (entry->kind == svn_node_file && strcmp(name, SVN_PROP_NEEDS_LOCK) == 0)
     {
       /* If the svn:needs-lock property was set to NULL, set the file
