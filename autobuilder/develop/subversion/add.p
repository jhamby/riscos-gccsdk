diff -ur subversion-1.5.5.orig/subversion/libsvn_client/add.c subversion-1.5.5/subversion/libsvn_client/add.c
--- subversion/libsvn_client/add.c	2008-05-21 22:24:24.000000000 +0100
+++ subversion/libsvn_client/add.c	2009-01-03 20:57:17.000000000 +0000
@@ -202,6 +202,15 @@
     }
 #endif
 
+    {
+      svn_string_t *filetype;
+      SVN_ERR (svn_io_get_file_filetype (&filetype, path, pool));
+      if (filetype)
+        apr_hash_set (autoprops.properties, SVN_PROP_FILETYPE,
+                      strlen (SVN_PROP_FILETYPE),
+                      filetype);
+    }
+
   *mimetype = autoprops.mimetype;
   return SVN_NO_ERROR;
 }
