--- subversion/libsvn_subr/io.c.orig	2009-08-28 20:38:14.000000000 +0200
+++ subversion/libsvn_subr/io.c	2009-08-28 20:37:07.000000000 +0200
@@ -24,6 +24,8 @@
 #include <unistd.h>
 #endif
 
+#include <unixlib/local.h>
+
 #ifndef APR_STATUS_IS_EPERM
 #include <errno.h>
 #ifdef EPERM
@@ -781,6 +783,8 @@
     apr_finfo_t finfo;
     const char *dst_apr;
     apr_status_t apr_err;
+    svn_string_t *filetype;
+    const char *src_tmp_apr;
 
     SVN_ERR(svn_io_file_open(&src_file, src, APR_READ, APR_OS_DEFAULT, pool));
     SVN_ERR(svn_io_file_info_get(&finfo, APR_FINFO_PROT, src_file, pool));
@@ -801,6 +805,11 @@
         return svn_error_wrap_apr(apr_err, _("Can't set permissions on '%s'"),
                                   svn_path_local_style(dst, pool));
       }
+
+      SVN_ERR (cstring_from_utf8(&src_tmp_apr, dst, pool));
+      SVN_ERR (svn_io_get_file_filetype(&filetype, src_tmp_apr, pool));
+      SVN_ERR (svn_io_set_file_filetype(dst_apr, filetype, pool));
+
   }
 #endif /* ! WIN32 */
 
@@ -1494,7 +1503,7 @@
                           const char *path,
                           apr_pool_t *pool)
 {
-#if defined(APR_HAS_USER) && !defined(WIN32)
+#if defined(APR_HAS_USER) && !defined(WIN32) && !defined(__riscos__)
   apr_finfo_t file_info;
   apr_status_t apr_err;
   apr_uid_t uid;
@@ -1528,6 +1537,59 @@
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
+                         const char *path,
+                         apr_pool_t *pool)
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
 
 /*** File locking. ***/
 /* Clear all outstanding locks on ARG, an open apr_file_t *. */
