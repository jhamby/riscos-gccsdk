--- gcc/gcc.c.orig	2007-07-03 00:58:30.000000000 +0200
+++ gcc/gcc.c	2007-06-28 01:13:39.000000000 +0200
@@ -343,7 +343,7 @@
 static void init_gcc_specs (struct obstack *, const char *, const char *,
 			    const char *);
 #endif
-#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
+#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(TARGET_CONVERT_FILENAME)
 static const char *convert_filename (const char *, int, int);
 #endif
 
@@ -1073,6 +1073,7 @@
    {"--imacros", "-imacros", "a"},
    {"--include", "-include", "a"},
    {"--include-barrier", "-I-", 0},
+   {"--include-cross-directory-after", "-icrossdirafter", "a"},
    {"--include-directory", "-I", "aj"},
    {"--include-directory-after", "-idirafter", "a"},
    {"--include-prefix", "-iprefix", "a"},
@@ -2978,7 +2983,7 @@
 
 const char **outfiles;
 
-#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
+#if defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined (TARGET_CONVERT_FILENAME)
 
 /* Convert NAME to a new name if it is the standard suffix.  DO_EXE
    is true if we should look for an executable suffix.  DO_OBJ
@@ -2988,6 +2993,9 @@
 convert_filename (const char *name, int do_exe ATTRIBUTE_UNUSED,
 		  int do_obj ATTRIBUTE_UNUSED)
 {
+#ifdef TARGET_CONVERT_FILENAME
+  TARGET_CONVERT_FILENAME(&obstack, name, do_exe, do_obj);
+#else
 #if defined(HAVE_TARGET_EXECUTABLE_SUFFIX)
   int i;
 #endif
@@ -3031,6 +3039,7 @@
 #endif
 
   return name;
+#endif /* ! TARGET_CONVERT_FILENAME */
 }
 #endif
 
@@ -3734,7 +3744,7 @@
 		    }
 		}
 #endif
-#if defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(HAVE_TARGET_OBJECT_SUFFIX)
+#if defined(HAVE_TARGET_EXECUTABLE_SUFFIX) || defined(HAVE_TARGET_OBJECT_SUFFIX) || defined(TARGET_CONVERT_FILENAME)
 	      if (p[1] == 0)
 		argv[i + 1] = convert_filename (argv[i + 1], ! have_c, 0);
 	      else
@@ -4084,7 +4094,7 @@
 	}
       else
 	{
-#ifdef HAVE_TARGET_OBJECT_SUFFIX
+#if defined (HAVE_TARGET_OBJECT_SUFFIX) || defined (TARGET_CONVERT_FILENAME)
 	  argv[i] = convert_filename (argv[i], 0, access (argv[i], F_OK));
 #endif
 
