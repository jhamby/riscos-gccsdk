--- gcc/collect2.c.orig	2008-03-31 23:53:11.000000000 +0200
+++ gcc/collect2.c	2008-04-01 00:04:55.000000000 +0200
@@ -47,6 +47,11 @@
 #include "intl.h"
 #include "version.h"
 
+#if defined(TARGET_RISCOSELF) && !defined(CROSS_COMPILE)
+#include <kernel.h>
+#include <unixlib/local.h>
+#endif
+
 /* On certain systems, we have code that works by scanning the object file
    directly.  But this code uses system-specific header files and library
    functions, so turn it off in a cross-compiler.  Likewise, the names of
@@ -179,6 +184,11 @@
 
 static int shared_obj;		        /* true if -shared */
 
+#ifdef TARGET_RISCOSELF
+static int riscos_libscl;		/* true if -mlibscl */
+static int riscos_module;		/* true if -mmodule */
+#endif
+
 static const char *c_file;		/* <xxx>.c for constructor/destructor list.  */
 static const char *o_file;		/* <xxx>.o for constructor/destructor list.  */
 #ifdef COLLECT_EXPORT_LIST
@@ -192,6 +202,7 @@
 static const char *ldd_file_name;	/* pathname of ldd (or equivalent) */
 #endif
 static const char *strip_file_name;		/* pathname of strip */
+static const char *elf2aif_file_name;	/* pathname of elf2aif */
 const char *c_file_name;	        /* pathname of gcc */
 static char *initname, *fininame;	/* names of init and fini funcs */
 
@@ -729,6 +740,87 @@
 	endp++;
     }
 }
+
+#ifdef TARGET_RISCOSELF
+#define DO_BINARY_POSTPROCESSING \
+  do_riscos_binary_postprocessing ();
+
+static void
+do_riscos_binary_postprocessing (void)
+{
+  if (!shared_obj && riscos_module)
+    {
+      /* Take the binary only part of ELF binary.  */
+#ifdef CROSS_COMPILE
+      /* When we are building the cross-compiler, we have strip-new built but
+	 it is not yet installed.  */
+      if (strip_file_name == NULL)
+	notice ("'%s' has not been found so therefore not used.\n", "strip");
+      else
+#endif
+	{
+	  char **real_strip_argv = xcalloc (sizeof (char *), 5);
+	  const char ** strip_argv = (const char **) real_strip_argv;
+
+	  strip_argv[0] = strip_file_name;
+	  strip_argv[1] = "-O";
+	  strip_argv[2] = "binary";
+	  strip_argv[3] = output_file;
+	  strip_argv[4] = NULL;
+	  fork_execute ("strip", real_strip_argv);
+	}
+
+#ifndef CROSS_COMPILE
+      {
+	char robuf[_POSIX_PATH_MAX];
+	/* Set filetype Module.  */
+	if (__riscosify_std (output_file, 0, robuf, sizeof (robuf), NULL))
+	  {
+	    _kernel_osfile_block bl;
+
+	    bl.load = 0xFFA; /* RISC OS Module */
+	    _kernel_osfile (18, robuf, &bl);
+	  }
+      }
+#endif
+    }
+  else if (!shared_obj && riscos_libscl)
+    {
+      /* Run elf2aif on the ELF binary.  */
+#ifdef CROSS_COMPILE
+      /* When we are building the cross-compiler, we don't have elf2aif in
+	 our build-tree as it gets built later. The test on CROSS_COMPILE is
+	 therefore a little bit too wide.  */
+      if (elf2aif_file_name == NULL)
+	notice ("'%s' has not been found so therefore not used.\n", "elf2aif");
+      else
+#endif
+	{
+	  char **real_elf2aif_argv = xcalloc (sizeof (char *), 3);
+	  const char ** elf2aif_argv = (const char **) real_elf2aif_argv;
+
+	  elf2aif_argv[0] = elf2aif_file_name;
+	  elf2aif_argv[1] = output_file;
+	  elf2aif_argv[2] = NULL;
+	  fork_execute ("elf2aif", real_elf2aif_argv);
+	}
+    }
+  else
+    {
+#ifndef CROSS_COMPILE
+      char robuf[_POSIX_PATH_MAX];
+      /* Set filetype ELF.  */
+      if (__riscosify_std (output_file, 0, robuf, sizeof (robuf), NULL))
+	{
+	  _kernel_osfile_block bl;
+
+	  bl.load = 0xE1F; /* ELF binary */
+	  _kernel_osfile (18, robuf, &bl);
+	}
+#endif
+    }
+}
+#endif
 
 /* Main program.  */
 
@@ -745,6 +837,7 @@
 #endif
   static const char *const strip_suffix = "strip";
   static const char *const gstrip_suffix = "gstrip";
+  static const char *const elf2aif_suffix = "elf2aif";
 
 #ifdef CROSS_COMPILE
   /* If we look for a program in the compiler directories, we just use
@@ -953,6 +1046,10 @@
   if (strip_file_name == 0)
     strip_file_name = find_a_file (&path, full_strip_suffix);
 
+  elf2aif_file_name = find_a_file (&cpath, elf2aif_suffix);
+  if (elf2aif_file_name == 0)
+    elf2aif_file_name = find_a_file (&path, elf2aif_suffix);
+
   /* Determine the full path name of the C compiler to use.  */
   c_file_name = getenv ("COLLECT_GCC");
   if (c_file_name == 0)
@@ -1015,6 +1112,12 @@
 	*c_ptr++ = xstrdup (q);
       if (strcmp (q, "-shared") == 0)
 	shared_obj = 1;
+#ifdef TARGET_RISCOSELF
+      if (strcmp (q, "-mlibscl") == 0)
+        riscos_libscl = 1;
+      else if (strcmp (q, "-mmodule") == 0)
+        riscos_module = 1;
+#endif
       if (*q == '-' && q[1] == 'B')
 	{
 	  *c_ptr++ = xstrdup (q);
@@ -1277,6 +1380,8 @@
 #endif
       fprintf (stderr, "strip_file_name     = %s\n",
 	       (strip_file_name ? strip_file_name : "not found"));
+      fprintf (stderr, "elf2aif_file_name   = %s\n",
+	       (elf2aif_file_name ? elf2aif_file_name : "not found"));
       fprintf (stderr, "c_file              = %s\n",
 	       (c_file ? c_file : "not found"));
       fprintf (stderr, "o_file              = %s\n",
@@ -1327,6 +1432,11 @@
 #endif
       maybe_unlink (c_file);
       maybe_unlink (o_file);
+
+#ifdef DO_BINARY_POSTPROCESSING
+      DO_BINARY_POSTPROCESSING
+#endif
+
       return 0;
     }
 
@@ -1381,6 +1491,11 @@
 #endif
       maybe_unlink (c_file);
       maybe_unlink (o_file);
+
+#ifdef DO_BINARY_POSTPROCESSING
+      DO_BINARY_POSTPROCESSING
+#endif
+
       return 0;
     }
 
@@ -1472,6 +1587,10 @@
   maybe_unlink (export_file);
 #endif
 
+#ifdef DO_BINARY_POSTPROCESSING
+  DO_BINARY_POSTPROCESSING
+#endif
+
   return 0;
 }
 
