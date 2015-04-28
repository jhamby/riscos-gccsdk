--- main.c.orig	2014-03-27 01:46:44.000000000 +1300
+++ main.c	2015-04-18 12:48:59.755201724 +1200
@@ -136,13 +136,22 @@
 static char outfile_path[MAXLINE];
 static int outfile_created = 0;
 static char *skelname = NULL;
-static int _stdout_closed = 0; /* flag to prevent double-fclose() on stdout. */
+static int _flout_closed = 0; /*flag to prevent double-fclose() on temp out*/
 const char *escaped_qstart = "[[]]M4_YY_NOOP[M4_YY_NOOP[M4_YY_NOOP[[]]";
 const char *escaped_qend   = "[[]]M4_YY_NOOP]M4_YY_NOOP]M4_YY_NOOP[[]]";
 
 /* For debugging. The max number of filters to apply to skeleton. */
 static int preproc_level = 1000;
 
+/* For RISC OS, temp files instead of pipes */ 
+char *flo;
+char *flho;
+char *m4o;
+FILE *flout;
+FILE *flhout;
+long int flsiz;
+long int flpos;
+
 int flex_main PROTO ((int argc, char *argv[]));
 int main PROTO ((int argc, char *argv[]));
 
@@ -161,9 +170,9 @@
 	 */
 	exit_status = setjmp (flex_main_jmp_buf);
 	if (exit_status){
-        if (stdout && !_stdout_closed && !ferror(stdout)){
-            fflush(stdout);
-            fclose(stdout);
+        if (flout && !_flout_closed && !ferror(flout)){
+            fsync((int)flout);
+            fclose(flout);
         }
         while (wait(&child_status) > 0){
             if (!WIFEXITED (child_status)
@@ -180,6 +189,18 @@
 
 	flexinit (argc, argv);
 
+	flo = mktemp("/tmp/flXXXXXX");
+	m4o = mktemp("/tmp/m4XXXXXX");
+	flout = fopen(flo, "w");
+        	if (flout == NULL)
+		fprintf(stderr,"error opening temp file %s",flo);
+	if (headerfilename) {
+		flho = mktemp("/tmp/fhXXXXXX");
+		flhout = fopen(flho, "w");
+        		if (flhout == NULL)
+		fprintf(stderr,"error opening temp file %s",flho);	
+		}
+
 	readin ();
 
 	skelout ();
@@ -199,6 +220,50 @@
 	/* Generate the C state transition tables from the DFA. */
 	make_tables ();
 
+	/* RISC OS -End of writing to body, keep size in case we need a header file */
+		fseek(flout,0,SEEK_END);
+		flsiz = (ftell(flout)-flpos);
+		if (ferror (flout))
+		fprintf(stderr,"error writing c temp file %s",flo);
+		else if (fclose (flout))
+		fprintf(stderr,"error closing output file %s",flo);
+
+	output_chain = filter_create_int(NULL, filter_empty, NULL);
+	filter_create_ext(output_chain, "m4", "-P", flo, ">", m4o, 0);
+	filter_apply_chain(output_chain);
+	filter_fix_linedirs (output_chain);
+
+	if (headerfilename) {
+		char buf[flsiz];
+		flout = fopen (flo, "r");
+		if (flout == NULL)
+		fprintf(stderr,"error opening temp file %s",flo);
+		fseek (flout,flpos,SEEK_SET);
+ 		fread (buf, flsiz, 1, flout);
+	 	if (fclose (flout))
+			fprintf(stderr,"error closing output file %s",flo);
+
+	fwrite (buf, flsiz, 1, flhout);
+	
+	/* write header file ending */
+	fprintf (flhout, "\n#line 4000 \"M4_YY_OUTFILE_NAME\"\n");
+	fprintf (flhout, "#undef %sIN_HEADER\n", prefix);
+	fprintf (flhout, "#endif /* %sHEADER_H */\n", prefix);
+	fputs ("m4_undefine( [[M4_YY_IN_HEADER]])m4_dnl\n", flhout);
+
+	if (fclose (flhout))
+		fprintf(stderr,"error closing temp file %s",flho);
+
+	output_chain = filter_create_int(NULL, filter_empty, NULL);
+	filter_create_ext(output_chain, "m4", "-P", flho, ">", m4o, 0);
+	filter_apply_chain(output_chain);
+ 	filter_fix_linedirs (output_chain);
+
+	remove(flho);
+	}
+	remove(flo);
+	remove(m4o);
+
 	/* Note, flexend does not return.  It exits with its argument
 	 * as status.
 	 */
@@ -334,8 +399,6 @@
     if (extra_type)
         buf_m4_define( &m4defs_buf, "M4_EXTRA_TYPE_DEFS", extra_type);
 
-	if (!use_stdout) {
-		FILE   *prev_stdout;
 
 		if (!did_outfilename) {
 			char   *suffix;
@@ -351,28 +414,20 @@
 			outfilename = outfile_path;
 		}
 
-		prev_stdout = freopen (outfilename, "w+", stdout);
-
-		if (prev_stdout == NULL)
-			lerrsf (_("could not create %s"), outfilename);
-
 		outfile_created = 1;
-	}
-
 
     /* Setup the filter chain. */
     output_chain = filter_create_int(NULL, filter_tee_header, headerfilename);
-    if ( !(m4 = getenv("M4")))
-        m4 = M4;
-    filter_create_ext(output_chain, m4, "-P", 0);
-    filter_create_int(output_chain, filter_fix_linedirs, NULL);
 
     /* For debugging, only run the requested number of filters. */
-    if (preproc_level > 0) {
-        filter_truncate(output_chain, preproc_level);
-        filter_apply_chain(output_chain);
-    }
-    yyout = stdout;
+    if (preproc_level > 0)
+	 filter_truncate(output_chain, preproc_level);
+	 filter_apply_chain(output_chain);
+
+	fseek(flout,0,SEEK_END);
+	flpos = ftell(flout);
+
+    yyout = flout;
 
 
 	/* always generate the tablesverify flag. */
@@ -434,7 +489,7 @@
     buf_m4_define(&m4defs_buf, "M4_YY_PREFIX", prefix);
 
 	if (did_outfilename)
-		line_directive_out (stdout, 0);
+		line_directive_out (flout, 0);
 
 	if (do_yylineno)
 		buf_m4_define (&m4defs_buf, "M4_YY_USE_LINENO", NULL);
@@ -469,7 +524,7 @@
         outn((char*) top_buf.elts);
 
     /* Dump the m4 definitions. */
-    buf_print_strings(&m4defs_buf, stdout);
+    buf_print_strings(&m4defs_buf, flout);
     m4defs_buf.nelts = 0; /* memory leak here. */
 
     /* Place a bogus line directive, it will be fixed in the filter. */
@@ -697,11 +752,11 @@
 #endif
 
 	if (exit_status != 0 && outfile_created) {
-		if (ferror (stdout))
+		if (ferror (flout))
 			lerrsf (_("error writing output file %s"),
 				outfilename);
 
-		else if ((_stdout_closed = 1) && fclose (stdout))
+		else if ((_flout_closed = 1) && fclose (flout))
 			lerrsf (_("error closing output file %s"),
 				outfilename);
 
