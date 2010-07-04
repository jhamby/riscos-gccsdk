--- gcc/gcc.c.orig	2006-11-07 15:26:21.000000000 +0100
+++ gcc/gcc.c	2010-07-04 19:34:09.936904419 +0200
@@ -343,7 +343,7 @@ static void fatal_error (int);
 static void init_gcc_specs (struct obstack *, const char *, const char *,
 			    const char *);
 #endif
-#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
+#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(TARGET_CONVERT_FILENAME)
 static const char *convert_filename (const char *, int, int);
 #endif
 
@@ -439,8 +439,8 @@ or with constant text in a single argume
 	SUFFIX characters following %O as they would following, for
 	example, `.o'.
  %I	Substitute any of -iprefix (made from GCC_EXEC_PREFIX), -isysroot
-	(made from TARGET_SYSTEM_ROOT), and -isystem (made from COMPILER_PATH
-	and -B options) as necessary.
+	(made from TARGET_SYSTEM_ROOT), -isystem (made from COMPILER_PATH
+	and -B options) and -imultilib as necessary.
  %s     current argument is the name of a library or startup file of some sort.
         Search for that file in a standard list of directories
 	and substitute the full name found.
@@ -2978,7 +2978,7 @@ static int added_libraries;
 
 const char **outfiles;
 
-#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
+#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined (TARGET_CONVERT_FILENAME)
 
 /* Convert NAME to a new name if it is the standard suffix.  DO_EXE
    is true if we should look for an executable suffix.  DO_OBJ
@@ -2988,6 +2988,9 @@ static const char *
 convert_filename (const char *name, int do_exe ATTRIBUTE_UNUSED,
 		  int do_obj ATTRIBUTE_UNUSED)
 {
+#ifdef TARGET_CONVERT_FILENAME
+  TARGET_CONVERT_FILENAME(&obstack, name, do_exe, do_obj);
+#else
 #if defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
   int i;
 #endif
@@ -3031,6 +3034,7 @@ convert_filename (const char *name, int 
 #endif
 
   return name;
+#endif /* ! TARGET_CONVERT_FILENAME */
 }
 #endif
 
@@ -3737,7 +3741,7 @@ warranty; not even for MERCHANTABILITY o
 		    }
 		}
 #endif
-#if defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(HAVE_TARGET_OBJECT_SUFFIX)
+#if defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(TARGET_CONVERT_FILENAME)
 	      if (p[1] == 0)
 		argv[i + 1] = convert_filename (argv[i + 1], ! have_c, 0);
 	      else
@@ -4087,7 +4091,7 @@ warranty; not even for MERCHANTABILITY o
 	}
       else
 	{
-#ifdef HAVE_TARGET_OBJECT_SUFFIX
+#if defined (HAVE_TARGET_OBJECT_SUFFIX) || defined (TARGET_CONVERT_FILENAME)
 	  argv[i] = convert_filename (argv[i], 0, access (argv[i], F_OK));
 #endif
 
@@ -4888,6 +4892,15 @@ do_spec_1 (const char *spec, int inswitc
 	    {
 	      struct prefix_list *pl = include_prefixes.plist;
 
+	      if (multilib_dir)
+		{
+		  do_spec_1 ("-imultilib", 1, NULL);
+		  /* Make this a separate argument.  */
+		  do_spec_1 (" ", 0, NULL);
+		  do_spec_1 (multilib_dir, 1, NULL);
+		  do_spec_1 (" ", 0, NULL);
+		}
+
 	      if (gcc_exec_prefix)
 		{
 		  do_spec_1 ("-iprefix", 1, NULL);
