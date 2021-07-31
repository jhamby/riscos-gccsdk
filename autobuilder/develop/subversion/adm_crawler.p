--- subversion/libsvn_wc/adm_crawler.c.orig	2009-05-04 13:56:51.000000000 +0200
+++ subversion/libsvn_wc/adm_crawler.c	2009-08-13 20:00:15.000000000 +0200
@@ -120,6 +120,7 @@
 
   /* If necessary, tweak the new working file's executable bit. */
   SVN_ERR(svn_wc__maybe_set_executable(NULL, file_path, adm_access, pool));
+  SVN_ERR(svn_wc__maybe_set_filetype (NULL, file_path, adm_access, pool));
 
   /* Remove any text conflict */
   SVN_ERR(svn_wc_resolved_conflict4(file_path, adm_access, TRUE, FALSE,
