--- src/gen.c.orig	2017-01-26 05:03:28.000000000 +1300
+++ src/gen.c	2020-09-05 20:25:08.802831738 +1200
@@ -33,7 +33,8 @@
 
 #include "flexdef.h"
 #include "tables.h"
-
+/* RISC OS temp file instead of stdout  */
+extern FILE* flout;
 
 /* declare functions that have forward references */
 
@@ -1858,7 +1859,7 @@
 
 	out (&action_array[defs1_offset]);
 
-	line_directive_out (stdout, 0);
+	line_directive_out (flout, 0);
 
 	skelout ();		/* %% [5.0] - break point in skel */
 
@@ -1927,7 +1928,7 @@
 	/* Copy prolog to output file. */
 	out (&action_array[prolog_offset]);
 
-	line_directive_out (stdout, 0);
+	line_directive_out (flout, 0);
 
 	skelout ();		/* %% [8.0] - break point in skel */
 
@@ -2062,7 +2063,7 @@
 	gen_bu_action ();
 	out (&action_array[action_offset]);
 
-	line_directive_out (stdout, 0);
+	line_directive_out (flout, 0);
 
 	/* generate cases for any missing EOF rules */
 	for (i = 1; i <= lastsc; ++i)
@@ -2150,15 +2151,15 @@
 
 	/* Copy remainder of input to output. */
 
-	line_directive_out (stdout, 1);
+	line_directive_out (flout, 1);
 
 	if (sectnum == 3) {
 		OUT_BEGIN_CODE ();
                 if (!no_section3_escape)
-                   fputs("[[", stdout);
+                   fputs("[[", flout);
 		(void) flexscan ();	/* copy remainder of input to output */
                 if (!no_section3_escape)
-                   fputs("]]", stdout);
+                   fputs("]]", flout);
 		OUT_END_CODE ();
 	}
 }
