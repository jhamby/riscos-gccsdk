diff -ur subversion.orig/clients/cmdline/main.c subversion/clients/cmdline/main.c
--- subversion.orig/clients/cmdline/main.c	2005-07-28 23:08:40.000000000 +0100
+++ subversion/clients/cmdline/main.c	2006-03-22 20:52:31.000000000 +0000
@@ -48,6 +48,15 @@
 
 #include "svn_private_config.h"
 
+#ifdef __riscos__
+
+#include <unixlib/local.h>
+
+int __riscosify_control = __RISCOSIFY_STRICT_UNIX_SPECS;
+int __feature_imagefs_is_file = 1;
+
+#endif
+
 
 /*** Option Processing ***/
 
@@ -823,6 +832,11 @@
   if (svn_cmdline_init ("svn", stderr) != EXIT_SUCCESS)
     return EXIT_FAILURE;
 
+#ifdef __riscos__
+  if (getenv("svn$filetypeext"))
+    __riscosify_control |= __RISCOSIFY_FILETYPE_EXT | __RISCOSIFY_FILETYPE_NOT_SET;
+#endif
+
   /* Create our top-level pool.  Use a seperate mutexless allocator,
    * given this application is single threaded.
    */
Only in subversion/clients/cmdline: main.c.orig
diff -ur subversion.orig/include/svn_io.h subversion/include/svn_io.h
--- subversion.orig/include/svn_io.h	2005-04-04 22:04:29.000000000 +0100
+++ subversion/include/svn_io.h	2006-03-22 20:52:30.000000000 +0000
@@ -298,6 +298,10 @@
                                          svn_boolean_t ignore_enoent,
                                          apr_pool_t *pool);
 
+svn_error_t *svn_io_set_file_filetype (const char *path,
+                                       const svn_string_t *filetype,
+                                       apr_pool_t *pool);
+
 /** Determine whether a file is executable by the current user.  
  * Set @a *executable to @c TRUE if the file @a path is executable by the 
  * current user, otherwise set it to @c FALSE.  
@@ -308,6 +312,9 @@
                                        const char *path, 
                                        apr_pool_t *pool);
 
+svn_error_t *svn_io_get_file_filetype(svn_string_t **filetype,
+                                      const char *path,
+                                      apr_pool_t *pool);
 
 /** Read a line from @a file into @a buf, but not exceeding @a *limit bytes.
  * Does not include newline, instead '\\0' is put there.
Only in subversion/include: svn_io.h.orig
diff -ur subversion.orig/include/svn_props.h subversion/include/svn_props.h
--- subversion.orig/include/svn_props.h	2005-04-04 19:16:07.000000000 +0100
+++ subversion/include/svn_props.h	2006-03-22 20:52:30.000000000 +0000
@@ -179,6 +179,9 @@
 /** Set to either TRUE or FALSE if we want a file to be executable or not. */
 #define SVN_PROP_EXECUTABLE  SVN_PROP_PREFIX "executable"
 
+#define SVN_PROP_FILETYPE  SVN_PROP_PREFIX "riscosfiletype"
+
+
 /** The value to force the executable property to when set */
 #define SVN_PROP_EXECUTABLE_VALUE "*"
 
diff -ur subversion.orig/libsvn_client/add.c subversion/libsvn_client/add.c
--- subversion.orig/libsvn_client/add.c	2005-03-22 05:32:19.000000000 +0000
+++ subversion/libsvn_client/add.c	2006-03-22 20:52:30.000000000 +0000
@@ -193,6 +193,15 @@
                       svn_string_create ("", pool));
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
Only in subversion/libsvn_client: add.c.orig
diff -ur subversion.orig/libsvn_client/export.c subversion/libsvn_client/export.c
--- subversion.orig/libsvn_client/export.c	2005-08-03 23:20:13.000000000 +0100
+++ subversion/libsvn_client/export.c	2006-03-22 20:52:30.000000000 +0000
@@ -103,7 +103,7 @@
   svn_subst_eol_style_t style;
   apr_hash_t *props;
   const char *base;
-  svn_string_t *eol_style, *keywords, *executable, *externals, *special;
+          svn_string_t *eol_style, *keywords, *filetype, *executable, *externals, *special;
   const char *eol = NULL;
   svn_boolean_t local_mod = FALSE;
   apr_time_t tm;
@@ -149,6 +149,8 @@
                            APR_HASH_KEY_STRING);
   executable = apr_hash_get (props, SVN_PROP_EXECUTABLE,
                              APR_HASH_KEY_STRING);
+          filetype = apr_hash_get (props, SVN_PROP_FILETYPE,
+                                   APR_HASH_KEY_STRING);
   externals = apr_hash_get (props, SVN_PROP_EXTERNALS,
                             APR_HASH_KEY_STRING);
   special = apr_hash_get (props, SVN_PROP_SPECIAL,
@@ -201,6 +203,8 @@
   if (executable)
     SVN_ERR (svn_io_set_file_executable (to, TRUE, 
                                          FALSE, pool));
+  if (filetype)
+    SVN_ERR (svn_io_set_file_filetype (to, filetype, pool));
 
   if (! special)
     SVN_ERR (svn_io_set_file_affected_time (tm, to, pool));
@@ -431,6 +435,7 @@
   const svn_string_t *eol_style_val;
   const svn_string_t *keywords_val;
   const svn_string_t *executable_val;
+  const svn_string_t *filetype_val;
   svn_boolean_t special;
 
   /* Any keyword vals to be substituted */
@@ -626,6 +631,9 @@
   else if (strcmp (name, SVN_PROP_EXECUTABLE) == 0)
     fb->executable_val = svn_string_dup (value, fb->pool);
 
+  else if (strcmp (name, SVN_PROP_FILETYPE) == 0)
+    fb->filetype_val = svn_string_dup (value, fb->pool);
+
   /* Try to fill out the baton's keywords-structure too. */
   else if (strcmp (name, SVN_PROP_ENTRY_COMMITTED_REV) == 0)
     fb->revision = apr_pstrdup (fb->pool, value->data);
@@ -719,10 +727,13 @@
 
       SVN_ERR (svn_io_remove_file (fb->tmppath, pool));
     }
-      
+
   if (fb->executable_val)
     SVN_ERR (svn_io_set_file_executable (fb->path, TRUE, FALSE, pool));
 
+  if (fb->filetype_val)
+    SVN_ERR (svn_io_set_file_filetype (fb->path, fb->filetype_val, pool));
+
   if (fb->date && (! fb->special))
     SVN_ERR (svn_io_set_file_affected_time (fb->date, fb->path, pool));
 
Only in subversion/libsvn_client: export.c.orig
Only in subversion/libsvn_client: export.c.rej
diff -ur subversion.orig/libsvn_subr/io.c subversion/libsvn_subr/io.c
--- subversion.orig/libsvn_subr/io.c	2005-08-19 16:18:45.000000000 +0100
+++ subversion/libsvn_subr/io.c	2006-03-22 20:52:30.000000000 +0000
@@ -31,6 +31,8 @@
 #endif
 #endif
 
+#include <unixlib/local.h>
+
 #ifndef APR_STATUS_IS_EPERM
 #ifdef EPERM
 #define APR_STATUS_IS_EPERM(s)   ((s) == EPERM)
@@ -579,6 +581,7 @@
     {
       apr_file_t *s;
       apr_finfo_t finfo;
+      svn_string_t *filetype;
 
       SVN_ERR (svn_io_file_open (&s, src, APR_READ, APR_OS_DEFAULT, pool));
       SVN_ERR (svn_io_file_info_get (&finfo, APR_FINFO_PROT, s, pool));
@@ -599,6 +602,10 @@
             (apr_err, _("Can't set permissions on '%s'"),
              svn_path_local_style (dst_tmp, pool));
         }
+
+      SVN_ERR (svn_io_get_file_filetype(&filetype, src_apr, pool));
+      SVN_ERR (svn_io_set_file_filetype(dst_tmp_apr, filetype, pool));
+
     }
 #endif /* ! WIN32 */
 
@@ -1239,13 +1246,35 @@
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
+      __object_set_attrs (path_apr, buf, sizeof (buf), filetype_val, __ATTR_NOTSPECIFIED);
+    }
+
+  return SVN_NO_ERROR;
+}
+
 
 svn_error_t *
 svn_io_is_file_executable(svn_boolean_t *executable, 
                           const char *path, 
                           apr_pool_t *pool)
 {
-#if defined(APR_HAS_USER) && !defined(WIN32)
+#if defined(APR_HAS_USER) && !defined(WIN32) && !defined(__riscos__)
   apr_finfo_t file_info;
   apr_status_t apr_err;
   apr_uid_t uid;
@@ -1279,6 +1308,38 @@
   return SVN_NO_ERROR;
 }
 
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
Only in subversion/libsvn_subr: io.c.orig
diff -ur subversion.orig/libsvn_wc/adm_crawler.c subversion/libsvn_wc/adm_crawler.c
--- subversion.orig/libsvn_wc/adm_crawler.c	2005-04-11 14:03:20.000000000 +0100
+++ subversion/libsvn_wc/adm_crawler.c	2006-03-22 20:52:30.000000000 +0000
@@ -101,6 +101,7 @@
 
   /* If necessary, tweak the new working file's executable bit. */
   SVN_ERR (svn_wc__maybe_set_executable (NULL, file_path, adm_access, pool));
+  SVN_ERR (svn_wc__maybe_set_filetype (NULL, file_path, adm_access, pool));
 
   /* Remove any text conflict */
   SVN_ERR (svn_wc_resolved_conflict2 (file_path, adm_access, TRUE, FALSE,
Only in subversion/libsvn_wc: adm_crawler.c.orig
diff -ur subversion.orig/libsvn_wc/adm_ops.c subversion/libsvn_wc/adm_ops.c
--- subversion.orig/libsvn_wc/adm_ops.c	2005-05-07 02:25:05.000000000 +0100
+++ subversion/libsvn_wc/adm_ops.c	2006-03-22 20:52:30.000000000 +0000
@@ -1244,6 +1244,7 @@
               = &APR_ARRAY_IDX (propchanges, i, svn_prop_t);
             
             if ((! strcmp (propchange->name, SVN_PROP_EXECUTABLE))
+                || (! strcmp (propchange->name, SVN_PROP_FILETYPE))
                 || (! strcmp (propchange->name, SVN_PROP_KEYWORDS))
                 || (! strcmp (propchange->name, SVN_PROP_EOL_STYLE))
                 || (! strcmp (propchange->name, SVN_PROP_SPECIAL))
@@ -1363,6 +1364,8 @@
           SVN_ERR (svn_wc__maybe_set_read_only (NULL, fullpath, adm_access, pool));
           SVN_ERR (svn_wc__maybe_set_executable (NULL, fullpath, adm_access,
                                                  pool));
+          SVN_ERR (svn_wc__maybe_set_filetype (NULL, fullpath, adm_access,
+                                                 pool));
 
           /* Possibly set the timestamp to last-commit-time, rather
              than the 'now' time that already exists. */
Only in subversion/libsvn_wc: adm_ops.c.orig
diff -ur subversion.orig/libsvn_wc/log.c subversion/libsvn_wc/log.c
--- subversion.orig/libsvn_wc/log.c	2005-05-07 02:25:05.000000000 +0100
+++ subversion/libsvn_wc/log.c	2006-03-22 20:52:30.000000000 +0000
@@ -137,8 +137,10 @@
                                               adm_access, pool));
 
         /* After copying, set the file executable if props dictate. */
-        return svn_wc__maybe_set_executable (NULL, full_dest_path, adm_access,
-                                             pool);
+        SVN_ERR (svn_wc__maybe_set_executable (NULL, full_dest_path, adm_access,
+                                               pool));
+        return svn_wc__maybe_set_filetype (NULL, full_dest_path, adm_access,
+                                           pool);
       }
 
     case svn_wc__xfer_cp_and_detranslate:
@@ -312,6 +314,10 @@
        its timestamp, which is the point of returning this flag. :-) */
     *overwrote_working = TRUE;
 
+  SVN_ERR (svn_wc__maybe_set_filetype (&did_set, filepath, adm_access, pool));
+  if (did_set)
+    *overwrote_working = TRUE;
+
   /* Install the new text base if one is waiting. */
   if (kind == svn_node_file)  /* tmp_text_base exists */
     SVN_ERR (svn_wc__sync_text_base (filepath, pool));
@@ -731,7 +737,7 @@
   apr_pool_t *pool = loggy->pool; 
   int is_this_dir = (strcmp (name, SVN_WC_ENTRY_THIS_DIR) == 0);
   const char *rev = svn_xml_get_attr_value (SVN_WC__LOG_ATTR_REVISION, atts);
-  svn_boolean_t wc_root, overwrote_working = FALSE, remove_executable = FALSE;
+  svn_boolean_t wc_root, overwrote_working = FALSE, remove_executable = FALSE, change_filetype = FALSE;
   svn_boolean_t set_read_write = FALSE;
   const char *full_path;
   const char *pdir, *base_name;
@@ -742,6 +748,7 @@
   apr_time_t prop_time = 0; /* By default, don't override old stamp. */
   svn_node_kind_t kind;
   svn_wc_adm_access_t *adm_access;
+  const svn_string_t *filetype;
 
   /* Determine the actual full path of the affected item. */
   if (! is_this_dir)
@@ -1053,9 +1060,13 @@
                     && (propchange->value == NULL))
                   {
                     remove_executable = TRUE;
-                    break;
                   }
-              }                
+                if (! strcmp (propchange->name, SVN_PROP_FILETYPE))
+                  {
+                    filetype = svn_string_dup(propchange->value, pool);
+                    change_filetype = TRUE;
+                  }
+              }
 
             for (i = 0; i < propchanges->nelts; i++)
               {
@@ -1109,8 +1120,10 @@
           overwrote_working = TRUE; /* entry needs wc-file's timestamp  */
         }
 
+      if (change_filetype)
+        SVN_ERR (svn_io_set_file_filetype (full_path, filetype, pool));
+
 
-      
       /* If the working file was overwritten (due to re-translation)
          or touched (due to +x / -x), then use *that* textual
          timestamp instead. */
Only in subversion/libsvn_wc: log.c.orig
Only in subversion/libsvn_wc: log.c.rej
diff -ur subversion.orig/libsvn_wc/merge.c subversion/libsvn_wc/merge.c
--- subversion.orig/libsvn_wc/merge.c	2005-03-22 05:32:19.000000000 +0000
+++ subversion/libsvn_wc/merge.c	2006-03-22 20:52:30.000000000 +0000
@@ -376,6 +376,9 @@
     {
       SVN_ERR (svn_wc__maybe_set_executable (NULL, merge_target, adm_access,
                                              pool));
+  if (! dry_run)
+    SVN_ERR (svn_wc__maybe_set_filetype (NULL, merge_target, adm_access,
+                                         pool));
 
       SVN_ERR (svn_wc__maybe_set_read_only (NULL, merge_target,
                                             adm_access, pool));
Only in subversion/libsvn_wc: merge.c.orig
diff -ur subversion.orig/libsvn_wc/props.c subversion/libsvn_wc/props.c
--- subversion.orig/libsvn_wc/props.c	2005-05-09 23:23:58.000000000 +0100
+++ subversion/libsvn_wc/props.c	2006-03-22 20:52:30.000000000 +0000
@@ -904,6 +904,7 @@
                                   SVN_PROP_EXTERNALS,
                                   NULL };
   const char *dir_prohibit[] = { SVN_PROP_EXECUTABLE,
+                                 SVN_PROP_FILETYPE,
                                  SVN_PROP_KEYWORDS,
                                  SVN_PROP_EOL_STYLE,
                                  SVN_PROP_MIME_TYPE,
@@ -1115,6 +1116,11 @@
         }
     }
 
+  if (kind == svn_node_file && strcmp (name, SVN_PROP_FILETYPE) == 0)
+    {
+        SVN_ERR (svn_io_set_file_filetype (path, value, pool));
+    }
+
   err = svn_wc_prop_list (&prophash, path, adm_access, pool);
   if (err)
     return
Only in subversion/libsvn_wc: props.c.orig
diff -ur subversion.orig/libsvn_wc/translate.c subversion/libsvn_wc/translate.c
--- subversion.orig/libsvn_wc/translate.c	2005-05-11 19:10:45.000000000 +0100
+++ subversion/libsvn_wc/translate.c	2006-03-22 20:52:30.000000000 +0000
@@ -287,3 +287,25 @@
 
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
Only in subversion/libsvn_wc: translate.c.orig
diff -ur subversion.orig/libsvn_wc/translate.h subversion/libsvn_wc/translate.h
--- subversion.orig/libsvn_wc/translate.h	2005-05-07 02:25:05.000000000 +0100
+++ subversion/libsvn_wc/translate.h	2006-03-22 20:52:30.000000000 +0000
@@ -104,6 +104,11 @@
                               const char *path,
                               svn_wc_adm_access_t *adm_access,
                               apr_pool_t *pool);
+svn_error_t *
+svn_wc__maybe_set_filetype (svn_boolean_t *did_set,
+                            const char *path,
+                            svn_wc_adm_access_t *adm_access,
+                            apr_pool_t *pool);
 
 /* If the SVN_PROP_NEEDS_LOCK property is present and there is no
    lock token for the file in the working copy, set PATH to
Only in subversion/libsvn_wc: translate.h.orig
