--- subversion/libsvn_client/add.c.orig	2009-05-06 17:21:21.000000000 +0200
+++ subversion/libsvn_client/add.c	2009-08-13 20:20:25.000000000 +0200
@@ -243,6 +243,15 @@
                      svn_string_create("", pool));
     }
 
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
