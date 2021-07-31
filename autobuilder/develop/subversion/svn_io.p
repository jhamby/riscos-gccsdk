--- subversion/include/svn_io.h.orig	2009-05-25 16:26:01.000000000 +0200
+++ subversion/include/svn_io.h	2009-08-11 23:35:01.000000000 +0200
@@ -460,6 +460,12 @@
                            svn_boolean_t ignore_enoent,
                            apr_pool_t *pool);
 
+svn_error_t *
+svn_io_set_file_filetype (const char *path,
+                          const svn_string_t *filetype,
+                          apr_pool_t *pool);
+
+
 /** Determine whether a file is executable by the current user.
  * Set @a *executable to @c TRUE if the file @a path is executable by the
  * current user, otherwise set it to @c FALSE.
@@ -471,6 +477,10 @@
                           const char *path,
                           apr_pool_t *pool);
 
+svn_error_t *
+svn_io_get_file_filetype(svn_string_t **filetype,
+                         const char *path,
+                         apr_pool_t *pool);
 
 /** Read a line from @a file into @a buf, but not exceeding @a *limit bytes.
  * Does not include newline, instead '\\0' is put there.
