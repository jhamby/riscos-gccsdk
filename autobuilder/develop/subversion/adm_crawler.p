--- subversion/libsvn_wc/adm_crawler.c.orig	2007-10-14 14:11:29.000000000 -0700
+++ subversion/libsvn_wc/adm_crawler.c	2008-12-27 22:16:15.000000000 -0800
@@ -85,6 +85,7 @@
 
   /* If necessary, tweak the new working file's executable bit. */
   SVN_ERR(svn_wc__maybe_set_executable(NULL, file_path, adm_access, pool));
+  SVN_ERR(svn_wc__maybe_set_filetype (NULL, file_path, adm_access, pool));
 
   /* Remove any text conflict */
   SVN_ERR(svn_wc_resolved_conflict3(file_path, adm_access, TRUE, FALSE,
