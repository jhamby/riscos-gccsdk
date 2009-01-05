diff -ur subversion-1.5.5.orig/subversion/include/svn_io.h subversion-1.5.5/subversion/include/svn_io.h
--- subversion/include/svn_io.h	2008-06-11 06:26:41.000000000 +0100
+++ subversion/include/svn_io.h	2009-01-03 21:00:16.000000000 +0000
@@ -383,6 +383,11 @@
                                         svn_boolean_t ignore_enoent,
                                         apr_pool_t *pool);
 
+svn_error_t *svn_io_set_file_filetype (const char *path,
+                                       const svn_string_t *filetype,
+                                       apr_pool_t *pool);
+
+
 /** Determine whether a file is executable by the current user.
  * Set @a *executable to @c TRUE if the file @a path is executable by the
  * current user, otherwise set it to @c FALSE.
@@ -393,6 +398,9 @@
                                        const char *path,
                                        apr_pool_t *pool);
 
+svn_error_t *svn_io_get_file_filetype(svn_string_t **filetype,
+                                      const char *path,
+                                      apr_pool_t *pool);
 
 /** Read a line from @a file into @a buf, but not exceeding @a *limit bytes.
  * Does not include newline, instead '\\0' is put there.
diff -ur subversion-1.5.5.orig/subversion/libsvn_subr/io.c subversion-1.5.5/subversion/libsvn_subr/io.c
--- subversion/libsvn_subr/io.c	2008-10-08 20:28:59.000000000 +0100
+++ subversion/libsvn_subr/io.c	2009-01-03 21:24:45.000000000 +0000
@@ -25,6 +25,8 @@
 #include <unistd.h>
 #endif
 
+#include <unixlib/local.h>
+
 #ifndef APR_STATUS_IS_EPERM
 #include <errno.h>
 #ifdef EPERM
@@ -780,6 +782,7 @@
     {
       apr_file_t *s;
       apr_finfo_t finfo;
+      svn_string_t *filetype;
 
       SVN_ERR(svn_io_file_open(&s, src, APR_READ, APR_OS_DEFAULT, pool));
       SVN_ERR(svn_io_file_info_get(&finfo, APR_FINFO_PROT, s, pool));
@@ -800,6 +803,10 @@
             (apr_err, _("Can't set permissions on '%s'"),
              svn_path_local_style(dst_tmp, pool));
         }
+
+      SVN_ERR (svn_io_get_file_filetype(&filetype, src_apr, pool));
+      SVN_ERR (svn_io_set_file_filetype(dst_tmp_apr, filetype, pool));
+
     }
 #endif /* ! WIN32 */
 
@@ -1447,7 +1454,7 @@
                           const char *path,
                           apr_pool_t *pool)
 {
-#if defined(APR_HAS_USER) && !defined(WIN32)
+#if defined(APR_HAS_USER) && !defined(WIN32) && !defined(__riscos__)
   apr_finfo_t file_info;
   apr_status_t apr_err;
   apr_uid_t uid;
@@ -1481,6 +1488,60 @@
   return SVN_NO_ERROR;
 }
 
+svn_error_t *
+svn_io_set_file_filetype (const char *path,
+                          const svn_string_t *filetype,
+                          apr_pool_t *pool)
+{
+  if (!getenv ("svn$filetypeext"))
+    {
+      int filetype_val = 0xfff;
+      const char *path_apr;
+      char buf[_POSIX_PATH_MAX];
+
+      SVN_ERR (svn_path_cstring_from_utf8 (&path_apr, path, pool));
+
+      if (filetype && filetype->len > 0)
+      	filetype_val = 0xfff & strtol(filetype->data, NULL, 16);
+
+      __object_set_attrs (path_apr, buf, sizeof (buf), filetype_val, -1);
+    }
+
+  return SVN_NO_ERROR;
+}
+
+
+svn_error_t *
+svn_io_get_file_filetype(svn_string_t **filetype,
+                          const char *path,
+                          apr_pool_t *pool)
+{
+
+  if (getenv("svn$filetypeext"))
+    {
+      *filetype = NULL;
+    }
+  else
+    {
+      const char *path_apr;
+      char buf[_POSIX_PATH_MAX];
+      int filetype_val;
+
+      SVN_ERR (svn_path_cstring_from_utf8 (&path_apr, path, pool));
+
+      __object_get_attrs (path_apr, buf, sizeof (buf), NULL, &filetype_val, NULL, NULL, NULL, NULL);
+
+      if (filetype_val == __RISCOSIFY_FILETYPE_NOTFOUND)
+        filetype_val = 0xfff;
+
+      *filetype = svn_string_createf (pool, "%.3X", filetype_val);
+
+    }
+
+  return SVN_NO_ERROR;
+}
+
+
 
 /*** File locking. ***/
 /* Clear all outstanding locks on ARG, an open apr_file_t *. */
diff -ur subversion-1.5.5.orig/subversion/libsvn_wc/merge.c subversion-1.5.5/subversion/libsvn_wc/merge.c
--- subversion/libsvn_wc/merge.c	2008-12-17 20:20:41.000000000 +0000
+++ subversion/libsvn_wc/merge.c	2009-01-03 20:57:18.000000000 +0000
@@ -842,6 +842,9 @@
                                                 adm_access, merge_target,
                                                 pool));
 
+      SVN_ERR (svn_wc__maybe_set_filetype (NULL, merge_target, adm_access,
+                                           pool));
+
     }
 
   return SVN_NO_ERROR;
diff -ur subversion-1.5.5.orig/subversion/libsvn_wc/translate.c subversion-1.5.5/subversion/libsvn_wc/translate.c
--- subversion/libsvn_wc/translate.c	2007-12-27 20:42:46.000000000 +0000
+++ subversion/libsvn_wc/translate.c	2009-01-03 20:57:18.000000000 +0000
@@ -350,3 +350,25 @@
 
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
diff -ur subversion-1.5.5.orig/subversion/libsvn_wc/translate.h subversion-1.5.5/subversion/libsvn_wc/translate.h
--- subversion/libsvn_wc/translate.h	2006-02-15 21:30:49.000000000 +0000
+++ subversion/libsvn_wc/translate.h	2009-01-03 20:57:18.000000000 +0000
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
