Index: gcc/toplev.c
===================================================================
--- gcc/toplev.c	(revision 167353)
+++ gcc/toplev.c	(working copy)
@@ -95,6 +95,11 @@
 				   declarations for e.g. AIX 4.x.  */
 #endif
 
+#ifdef HAVE_cloog
+#include "cloog/version.h"
+#include "ppl_c.h"
+#endif
+
 static void general_init (const char *);
 static void do_compile (void);
 static void process_options (void);
@@ -686,6 +691,10 @@
 void
 print_version (FILE *file, const char *indent)
 {
+#ifdef HAVE_cloog
+  const char *ppl_version_string = "";
+#endif
+
   static const char fmt1[] =
 #ifdef __GNUC__
     N_("%s%s%s %sversion %s (%s)\n%s\tcompiled by GNU C version %s, ")
@@ -694,7 +703,12 @@
 #endif
     ;
   static const char fmt2[] =
-    N_("GMP version %s, MPFR version %s, MPC version %s\n");
+#ifndef HAVE_cloog
+    N_("GMP version %s, MPFR version %s, MPC version %s\n")
+#else
+    N_("GMP version %s, MPFR version %s, MPC version %s, PPL version %s, CLooG version %s\n")
+#endif
+    ;
   static const char fmt3[] =
     N_("%s%swarning: %s header version %s differs from library version %s.\n");
   static const char fmt4[] =
@@ -726,7 +740,11 @@
 #endif
   fprintf (file,
 	   file == stderr ? _(fmt2) : fmt2,
-	   GCC_GMP_STRINGIFY_VERSION, MPFR_VERSION_STRING, MPC_VERSION_STRING);
+	   GCC_GMP_STRINGIFY_VERSION, MPFR_VERSION_STRING, MPC_VERSION_STRING
+#ifdef HAVE_cloog
+	   , PPL_VERSION, CLOOG_VERSION
+#endif
+	  );
   if (strcmp (GCC_GMP_STRINGIFY_VERSION, gmp_version))
     fprintf (file,
 	     file == stderr ? _(fmt3) : fmt3,
@@ -742,6 +760,20 @@
 	     file == stderr ? _(fmt3) : fmt3,
 	     indent, *indent != 0 ? " " : "",
 	     "MPC", MPC_VERSION_STRING, mpc_get_version ());
+#ifdef HAVE_cloog
+  (void) ppl_version (&ppl_version_string);
+  if (strcmp (PPL_VERSION, ppl_version_string))
+    fprintf (file,
+	     file == stderr ? _(fmt3) : fmt3,
+	     indent, *indent != 0 ? " " : "",
+	     "PPL", PPL_VERSION, ppl_version_string);
+#define GCC_EXPECTED_CLOOG_VERSION "CLooG " CLOOG_VERSION " " CLOOG_BITS " bits"
+  if (strcmp (GCC_EXPECTED_CLOOG_VERSION, cloog_version ()))
+    fprintf (file,
+	     file == stderr ? _(fmt3) : fmt3,
+	     indent, *indent != 0 ? " " : "",
+	     "CLooG", GCC_EXPECTED_CLOOG_VERSION, cloog_version ());
+#endif
   fprintf (file,
 	   file == stderr ? _(fmt4) : fmt4,
 	   indent, *indent != 0 ? " " : "",
