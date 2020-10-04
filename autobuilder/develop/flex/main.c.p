--- src/main.c.orig	2020-09-11 21:22:22.614018000 +1200
+++ src/main.c	2020-09-28 21:53:41.959426173 +1300
@@ -126,12 +126,16 @@
 static char outfile_path[MAXLINE];
 static int outfile_created = 0;
 static char *skelname = NULL;
-static int _stdout_closed = 0; /* flag to prevent double-fclose() on stdout. */
+static int _flout_closed = 0; /* flag to prevent double-fclose() on flout. */
 const char *escaped_qstart = "]]M4_YY_NOOP[M4_YY_NOOP[M4_YY_NOOP[[";
 const char *escaped_qend   = "]]M4_YY_NOOP]M4_YY_NOOP]M4_YY_NOOP[[";
 
 /* For debugging. The max number of filters to apply to skeleton. */
 static int preproc_level = 1000;
+/* For RISC OS, temp files instead of fd pipes */ 
+char *flo;
+char *m4o;
+FILE *flout;
 
 int flex_main (int argc, char *argv[]);
 
@@ -148,10 +152,8 @@
 	 */
 	exit_status = setjmp (flex_main_jmp_buf);
 	if (exit_status){
-        if (stdout && !_stdout_closed && !ferror(stdout)){
-            fflush(stdout);
-            fclose(stdout);
-        }
+        if (flout && !_flout_closed && !ferror(flout))	
+            fclose(flout);  
         while (wait(&child_status) > 0){
             if (!WIFEXITED (child_status)
                 || WEXITSTATUS (child_status) != 0){
@@ -166,6 +168,12 @@
     }
 
 	flexinit (argc, argv);
+	
+	flo = mktemp("/tmp/flXXXXXX");
+	m4o = mktemp("/tmp/m4XXXXXX");
+	flout = fopen(flo, "w");
+        	if (flout == NULL)
+		fprintf(stderr,"error opening temp file %s",flo);				
 
 	readin ();
 
@@ -186,6 +194,20 @@
 	/* Generate the C state transition tables from the DFA. */
 	make_tables ();
 
+		if (fclose (flout)!=0 )
+		     lerr (_("error closing c temp file %s"),flo);	
+	output_chain->next = NULL;
+	filter_create_ext(output_chain, "m4", "-P",  flo, ">", m4o, 0);
+	filter_apply_chain(output_chain);  
+	     if (filter_fix_linedirs (output_chain)==0 && headerfilename) {
+    /* m4 the header file and output it */
+	filter_apply_chain(output_chain);
+	filter_fix_linedirs (output_chain);	
+		     }
+
+	remove(flo);
+	remove(m4o);
+	
 	/* Note, flexend does not return.  It exits with its argument
 	 * as status.
 	 */
@@ -313,9 +335,6 @@
 	if (extra_type)
 		buf_m4_define( &m4defs_buf, "M4_EXTRA_TYPE_DEFS", extra_type);
 
-	if (!use_stdout) {
-		FILE   *prev_stdout;
-
 		if (!did_outfilename) {
 			char   *suffix;
 
@@ -330,65 +349,13 @@
 			outfilename = outfile_path;
 		}
 
-		prev_stdout = freopen (outfilename, "w+", stdout);
-
-		if (prev_stdout == NULL)
-			lerr (_("could not create %s"), outfilename);
-
 		outfile_created = 1;
-	}
-
 
     /* Setup the filter chain. */
-    output_chain = filter_create_int(NULL, filter_tee_header, headerfilename);
-    if ( !(m4 = getenv("M4"))) {
-	    char *slash;
-		m4 = M4;
-		if ((slash = strrchr(M4, '/')) != NULL) {
-			m4 = slash+1;
-			/* break up $PATH */
-			const char *path = getenv("PATH");
-			if (!path) {
-				m4 = M4;
-			} else {
-				int m4_length = strlen(m4);
-				do {
-					size_t length = strlen(path);
-					struct stat sbuf;
-
-					const char *endOfDir = strchr(path, ':');
-					if (!endOfDir)
-						endOfDir = path+length;
-
-					{
-						char *m4_path = calloc(endOfDir-path + 1 + m4_length + 1, 1);
-
-						memcpy(m4_path, path, endOfDir-path);
-						m4_path[endOfDir-path] = '/';
-						memcpy(m4_path + (endOfDir-path) + 1, m4, m4_length + 1);
-						if (stat(m4_path, &sbuf) == 0 &&
-							(S_ISREG(sbuf.st_mode)) && sbuf.st_mode & S_IXUSR) {
-							m4 = m4_path;
-							break;
-						}
-						free(m4_path);
-					}
-					path = endOfDir+1;
-				} while (path[0]);
-				if (!path[0])
-				    m4 = M4;
-			}
-		}
-	}
-    filter_create_ext(output_chain, m4, "-P", 0);
-    filter_create_int(output_chain, filter_fix_linedirs, NULL);
+    output_chain = filter_create_int(NULL, filter_tee_header,headerfilename);
+	 filter_tee_header(output_chain);
 
-    /* For debugging, only run the requested number of filters. */
-    if (preproc_level > 0) {
-        filter_truncate(output_chain, preproc_level);
-        filter_apply_chain(output_chain);
-    }
-    yyout = stdout;
+    yyout = flout;
 
 
 	/* always generate the tablesverify flag. */
@@ -451,7 +418,7 @@
     buf_m4_define(&m4defs_buf, "M4_YY_PREFIX", prefix);
 
 	if (did_outfilename)
-		line_directive_out (stdout, 0);
+		line_directive_out (flout, 0);
 
 	if (do_yylineno)
 		buf_m4_define (&m4defs_buf, "M4_YY_USE_LINENO", NULL);
@@ -487,7 +454,7 @@
         outn((char*) top_buf.elts);
 
     /* Dump the m4 definitions. */
-    buf_print_strings(&m4defs_buf, stdout);
+    buf_print_strings(&m4defs_buf, flout);
     m4defs_buf.nelts = 0; /* memory leak here. */
 
     /* Place a bogus line directive, it will be fixed in the filter. */
@@ -710,16 +677,15 @@
 		if (ferror (header_out))
 			lerr (_("error creating header file %s"),
 				headerfilename);
-		fflush (header_out);
 		fclose (header_out);
 #endif
 
 	if (exit_status != 0 && outfile_created) {
-		if (ferror (stdout))
+		if (ferror (flout))
 			lerr (_("error writing output file %s"),
 				outfilename);
 
-		else if ((_stdout_closed = 1) && fclose (stdout))
+		else if ((_flout_closed = 1) && fclose (flout))
 			lerr (_("error closing output file %s"),
 				outfilename);
 
