diff -ur subversion-1.5.5.orig/subversion/libsvn_wc/props.c subversion-1.5.5/subversion/libsvn_wc/props.c
--- subversion/libsvn_wc/props.c	2008-08-27 16:31:53.000000000 +0100
+++ subversion/libsvn_wc/props.c	2009-01-03 20:57:18.000000000 +0000
@@ -2324,6 +2324,7 @@
                                   SVN_PROP_EXTERNALS,
                                   NULL };
   const char *dir_prohibit[] = { SVN_PROP_EXECUTABLE,
+                                 SVN_PROP_FILETYPE,
                                  SVN_PROP_KEYWORDS,
                                  SVN_PROP_EOL_STYLE,
                                  SVN_PROP_MIME_TYPE,
@@ -2512,6 +2513,11 @@
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
