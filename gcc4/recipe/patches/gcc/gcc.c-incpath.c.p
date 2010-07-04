--- gcc/c-incpath.c.orig	2010-07-04 16:15:17.274401880 +0200
+++ gcc/c-incpath.c	2010-07-04 16:14:32.714402060 +0200
@@ -45,8 +45,10 @@ Foundation, 51 Franklin Street, Fifth Fl
 # define INO_T_COPY(DEST, SRC) (DEST) = (SRC)
 #endif
 
+static const char dir_separator_str[] = { DIR_SEPARATOR, 0 };
+
 static void add_env_var_paths (const char *, int);
-static void add_standard_paths (const char *, const char *, int);
+static void add_standard_paths (const char *, const char *, const char *, int);
 static void free_path (struct cpp_dir *, int);
 static void merge_include_chains (cpp_reader *, int);
 static struct cpp_dir *remove_duplicates (cpp_reader *, struct cpp_dir *,
@@ -120,7 +122,8 @@ add_env_var_paths (const char *env_var, 
 
 /* Append the standard include chain defined in cppdefault.c.  */
 static void
-add_standard_paths (const char *sysroot, const char *iprefix, int cxx_stdinc)
+add_standard_paths (const char *sysroot, const char *iprefix,
+		    const char *imultilib, int cxx_stdinc)
 {
   const struct default_include *p;
   size_t len;
@@ -142,6 +145,8 @@ add_standard_paths (const char *sysroot,
 	      if (!strncmp (p->fname, cpp_GCC_INCLUDE_DIR, len))
 		{
 		  char *str = concat (iprefix, p->fname + len, NULL);
+		  if (p->multilib && imultilib)
+		    str = concat (str, dir_separator_str, imultilib, NULL);
 		  add_path (str, SYSTEM, p->cxx_aware, false);
 		}
 	    }
@@ -160,6 +165,9 @@ add_standard_paths (const char *sysroot,
 	  else
 	    str = update_path (p->fname, p->component);
 
+	  if (p->multilib && imultilib)
+	    str = concat (str, dir_separator_str, imultilib, NULL);
+
 	  add_path (str, SYSTEM, p->cxx_aware, false);
 	}
     }
@@ -340,6 +348,21 @@ add_path (char *path, int chain, int cxx
     if (*c == '\\') *c = '/';
 #endif
 
+#ifdef __riscos__
+  extern char *__unixify_std (const char *, char *, size_t, int);
+  char *temp;
+  temp = __unixify_std (path, NULL, 0, 0);
+  if (!temp)
+    {
+      fprintf (stderr, _("Unixify failed (%s), falling back to input path\n"),
+	       path);
+    }
+  else
+    {
+      free (path);
+      path = temp;
+    }
+#endif
   p = xmalloc (sizeof (cpp_dir));
   p->next = NULL;
   p->name = path;
@@ -357,8 +380,8 @@ add_path (char *path, int chain, int cxx
    removal, and registration with cpplib.  */
 void
 register_include_chains (cpp_reader *pfile, const char *sysroot,
-			 const char *iprefix, int stdinc, int cxx_stdinc,
-			 int verbose)
+			 const char *iprefix, const char *imultilib,
+			 int stdinc, int cxx_stdinc, int verbose)
 {
   static const char *const lang_env_vars[] =
     { "C_INCLUDE_PATH", "CPLUS_INCLUDE_PATH",
@@ -380,7 +403,7 @@ register_include_chains (cpp_reader *pfi
 
   /* Finally chain on the standard directories.  */
   if (stdinc)
-    add_standard_paths (sysroot, iprefix, cxx_stdinc);
+    add_standard_paths (sysroot, iprefix, imultilib, cxx_stdinc);
 
   target_c_incpath.extra_includes (sysroot, iprefix, stdinc);
 
