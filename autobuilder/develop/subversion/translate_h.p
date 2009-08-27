--- subversion/libsvn_wc/translate.h.orig	2009-08-12 00:12:39.000000000 +0200
+++ subversion/libsvn_wc/translate.h	2009-08-11 23:21:08.000000000 +0200
@@ -103,6 +103,12 @@
                              svn_wc_adm_access_t *adm_access,
                              apr_pool_t *pool);
 
+svn_error_t *
+svn_wc__maybe_set_filetype (svn_boolean_t *did_set,
+                            const char *path,
+                            svn_wc_adm_access_t *adm_access,
+                            apr_pool_t *pool);
+
 /* If the SVN_PROP_NEEDS_LOCK property is present and there is no
    lock token for the file in the working copy, set PATH to
    read-only. If DID_SET is non-null, then set *DID_SET to TRUE if
