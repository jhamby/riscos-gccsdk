--- subversion/libsvn_wc/merge.c.orig	2009-02-16 21:35:25.000000000 +0100
+++ subversion/libsvn_wc/merge.c	2009-08-12 00:05:32.000000000 +0200
@@ -1268,6 +1268,8 @@
                                                  merge_target, pool));
       SVN_ERR(svn_wc__loggy_maybe_set_readonly(log_accum, adm_access,
                                                merge_target, pool));
+      SVN_ERR (svn_wc__maybe_set_filetype (NULL, merge_target, adm_access,
+                                           pool));
     }
 
   return SVN_NO_ERROR;
