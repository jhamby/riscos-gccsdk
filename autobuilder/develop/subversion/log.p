--- subversion/libsvn_wc/log.c.orig	2009-03-20 15:50:14.000000000 +0100
+++ subversion/libsvn_wc/log.c	2009-08-11 22:46:36.000000000 +0200
@@ -328,6 +328,9 @@
         SVN_ERR(svn_wc__maybe_set_read_only(NULL, full_dest_path,
                                             adm_access, pool));
 
+        SVN_ERR(svn_wc__maybe_set_filetype(NULL, full_dest_path, adm_access,
+                                           pool));
+
         return svn_wc__maybe_set_executable(NULL, full_dest_path,
                                             adm_access, pool);
       }
@@ -492,6 +495,10 @@
         *overwrote_working = TRUE;
     }
 
+  SVN_ERR (svn_wc__maybe_set_filetype (&did_set, filepath, adm_access, pool));
+  if (did_set)
+    *overwrote_working = TRUE;
+
   /* Install the new text base if one is waiting. */
   if (kind == svn_node_file)  /* tmp_text_base exists */
     SVN_ERR(svn_wc__sync_text_base(filepath, pool));
@@ -1044,7 +1051,7 @@
   apr_pool_t *pool = loggy->pool;
   int is_this_dir = (strcmp(name, SVN_WC_ENTRY_THIS_DIR) == 0);
   const char *rev = svn_xml_get_attr_value(SVN_WC__LOG_ATTR_REVISION, atts);
-  svn_boolean_t wc_root, remove_executable = FALSE;
+  svn_boolean_t wc_root, remove_executable = FALSE, change_filetype = FALSE;
   svn_boolean_t set_read_write = FALSE;
   const char *full_path;
   const char *pdir, *base_name;
@@ -1054,6 +1061,7 @@
   svn_wc_adm_access_t *adm_access;
   svn_boolean_t prop_mods;
   apr_uint64_t modify_flags = 0;
+  const svn_string_t *filetype;
 
   /* Determine the actual full path of the affected item. */
   if (! is_this_dir)
@@ -1235,6 +1243,9 @@
               else if ((! strcmp(propchange->name, SVN_PROP_NEEDS_LOCK))
                        && (propchange->value == NULL))
                 set_read_write = TRUE;
+              else if ((! strcmp(propchange->name, SVN_PROP_FILETYPE))
+                       && (propchange->value == NULL))
+                change_filetype = TRUE;
             }
         }
 
@@ -1286,6 +1297,9 @@
           const char *basef;
           apr_finfo_t basef_finfo;
 
+          if (change_filetype)
+            SVN_ERR(svn_io_set_file_filetype (full_path, filetype, pool));
+
           /* If the working file was overwritten (due to re-translation)
              or touched (due to +x / -x), then use *that* textual
              timestamp instead. */
