--- gen.c.orig	2015-04-18 13:42:13.531932000 +1200
+++ gen.c	2015-04-18 13:52:48.365482000 +1200
@@ -35,6 +35,7 @@
 #include "tables.h"
 
 
+extern FILE* flout;
 /* declare functions that have forward references */
 
 void gen_next_state PROTO ((int));
@@ -1867,7 +1868,7 @@
 
 	out (&action_array[defs1_offset]);
 
-	line_directive_out (stdout, 0);
+	line_directive_out (flout, 0);
 
 	skelout ();		/* %% [5.0] - break point in skel */
 
@@ -1936,7 +1937,7 @@
 	/* Copy prolog to output file. */
 	out (&action_array[prolog_offset]);
 
-	line_directive_out (stdout, 0);
+	line_directive_out (flout, 0);
 
 	skelout ();		/* %% [8.0] - break point in skel */
 
@@ -2071,7 +2072,7 @@
 	gen_bu_action ();
 	out (&action_array[action_offset]);
 
-	line_directive_out (stdout, 0);
+	line_directive_out (flout, 0);
 
 	/* generate cases for any missing EOF rules */
 	for (i = 1; i <= lastsc; ++i)
@@ -2159,7 +2160,7 @@
 
 	/* Copy remainder of input to output. */
         linenum--;
-	line_directive_out (stdout, 1);
+	line_directive_out (flout, 1);
 
 	if (sectnum == 3) {
 		OUT_BEGIN_CODE ();
