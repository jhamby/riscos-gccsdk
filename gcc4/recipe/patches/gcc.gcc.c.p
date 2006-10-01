--- gcc/gcc.c.orig	2006-09-24 23:08:50.000000000 +0200
+++ gcc/gcc.c	2006-09-24 23:09:04.000000000 +0200
@@ -343,7 +343,7 @@
 static void init_gcc_specs (struct obstack *, const char *, const char *,
 			    const char *);
 #endif
-#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
+#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(TARGET_CONVERT_FILENAME)
 static const char *convert_filename (const char *, int, int);
 #endif
 
@@ -1073,6 +1073,10 @@
    {"--imacros", "-imacros", "a"},
    {"--include", "-include", "a"},
    {"--include-barrier", "-I-", 0},
+   /* NAB++ */
+   {"--include-cross-directory-after", "-icrossdirafter", "a"},
+   {"--include-cross-directory-system", "-icrossdirsystem", "a"},
+   /* NAB-- */
    {"--include-directory", "-I", "aj"},
    {"--include-directory-after", "-idirafter", "a"},
    {"--include-prefix", "-iprefix", "a"},
@@ -1708,7 +1712,11 @@
 	    init_gcc_specs (&obstack,
 			    "-lgcc_s"
 #ifdef USE_LIBUNWIND_EXCEPTIONS
+# ifdef HAVE_LD_STATIC_DYNAMIC
+			    " %{!static:-Bstatic} -lunwind %{!static:-Bdynamic}"
+# else
 			    " -lunwind"
+# endif
 #endif
 			    ,
 			    "-lgcc",
@@ -2978,7 +2986,7 @@
 
 const char **outfiles;
 
-#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
+#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined (TARGET_CONVERT_FILENAME)
 
 /* Convert NAME to a new name if it is the standard suffix.  DO_EXE
    is true if we should look for an executable suffix.  DO_OBJ
@@ -2988,6 +2996,9 @@
 convert_filename (const char *name, int do_exe ATTRIBUTE_UNUSED,
 		  int do_obj ATTRIBUTE_UNUSED)
 {
+#ifdef TARGET_CONVERT_FILENAME
+  TARGET_CONVERT_FILENAME(&obstack, name, do_exe, do_obj);
+#else
 #if defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
   int i;
 #endif
@@ -3031,6 +3042,7 @@
 #endif
 
   return name;
+#endif /* ! TARGET_CONVERT_FILENAME */
 }
 #endif
 
@@ -3241,6 +3253,10 @@
      see if we can create it from the pathname specified in argv[0].  */
 
   gcc_libexec_prefix = standard_libexec_prefix;
+
+  /* NAB++ */
+#if 0
+  /* NAB-- */
 #ifndef VMS
   /* FIXME: make_relative_prefix doesn't yet work for VMS.  */
   if (!gcc_exec_prefix)
@@ -3259,6 +3275,9 @@
 					       standard_libexec_prefix);
 #else
 #endif
+  /* NAB++ */
+#endif
+  /* NAB-- */
 
   if (gcc_exec_prefix)
     {
@@ -3734,7 +3753,7 @@
 		    }
 		}
 #endif
-#if defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(HAVE_TARGET_OBJECT_SUFFIX)
+#if defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(TARGET_CONVERT_FILENAME)
 	      if (p[1] == 0)
 		argv[i + 1] = convert_filename (argv[i + 1], ! have_c, 0);
 	      else
@@ -4084,7 +4103,7 @@
 	}
       else
 	{
-#ifdef HAVE_TARGET_OBJECT_SUFFIX
+#if defined (HAVE_TARGET_OBJECT_SUFFIX) || defined (TARGET_CONVERT_FILENAME)
 	  argv[i] = convert_filename (argv[i], 0, access (argv[i], F_OK));
 #endif
 
@@ -6626,10 +6645,17 @@
 	  if (s == NULL)
 	    linker_name_spec = "ld";
 	}
+
+/* NAB++ */
+#ifndef __riscos__
+/* NAB-- */
       /* Rebuild the COMPILER_PATH and LIBRARY_PATH environment variables
 	 for collect.  */
       putenv_from_prefixes (&exec_prefixes, "COMPILER_PATH");
       putenv_from_prefixes (&startfile_prefixes, LIBRARY_PATH_ENV);
+/* NAB++ */
+#endif
+/* NAB-- */
 
       value = do_spec (link_command_spec);
       if (value < 0)
