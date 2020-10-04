--- src/filter.c.orig	2020-09-11 14:12:39.728233000 +1200
+++ src/filter.c	2020-09-28 21:59:30.387833569 +1300
@@ -27,7 +27,9 @@
     "`errprint(Flex requires GNU M4. Set the PATH or set the M4 environment variable to its path name.)"
     " m4exit(2)')\n";
 
-
+bool h_out = false;
+extern FILE *flout;
+extern char *flo; 
 /** global chain. */
 struct filter *output_chain = NULL;
 
@@ -64,7 +66,7 @@
 
 
 	/* allocate argv, and populate it with the argument list. */
-	max_args = 8;
+	max_args = 11;
 	f->argv = malloc(sizeof(char *) * (size_t) (max_args + 1));
 	if (!f->argv)
 		flexerror(_("malloc failed (f->argv) in filter_create_ext"));
@@ -125,9 +127,8 @@
  */
 bool filter_apply_chain (struct filter * chain)
 {
-	int     pid, pipes[2];
-
-
+	pid_t pid;
+	
 	/* Tricky recursion, since we want to begin the chain
 	 * at the END. Why? Because we need all the forked processes
 	 * to be children of the main flex process.
@@ -137,37 +138,12 @@
 	else
 		return true;
 
-	/* Now we are the right-most unprocessed link in the chain.
-	 */
-
-	fflush (stdout);
-	fflush (stderr);
-
-
-	if (pipe (pipes) == -1)
-		flexerror (_("pipe failed"));
-
 	if ((pid = fork ()) == -1)
 		flexerror (_("fork failed"));
 
 	if (pid == 0) {
 		/* child */
 
-        /* We need stdin (the FILE* stdin) to connect to this new pipe.
-         * There is no portable way to set stdin to a new file descriptor,
-         * as stdin is not an lvalue on some systems (BSD).
-         * So we dup the new pipe onto the stdin descriptor and use a no-op fseek
-         * to sync the stream. This is a Hail Mary situation. It seems to work.
-         */
-		close (pipes[1]);
-clearerr(stdin);
-		if (dup2 (pipes[0], fileno (stdin)) == -1)
-			flexfatal (_("dup2(pipes[0],0)"));
-		close (pipes[0]);
-        fseek (stdin, 0, SEEK_CUR);
-        ungetc(' ', stdin); /* still an evil hack, but one that works better */
-        (void)fgetc(stdin); /* on NetBSD than the fseek attempt does */
-
 		/* run as a filter, either internally or by exec */
 		if (chain->filter_func) {
 			int     r;
@@ -187,12 +163,11 @@
 	}
 
 	/* Parent */
-	close (pipes[0]);
-	if (dup2 (pipes[1], fileno (stdout)) == -1)
-		flexfatal (_("dup2(pipes[1],1)"));
-	close (pipes[1]);
-    fseek (stdout, 0, SEEK_CUR);
-
+  int child_status;
+    while (wait(&child_status) > 0){
+       if (!WIFEXITED (child_status) || WEXITSTATUS (child_status) != 0)
+         lerr_fatal (_("flex: child error from m4"));     
+		}
 	return true;
 }
 
@@ -217,108 +192,56 @@
 	return len;
 }
 
-/** Splits the chain in order to write to a header file.
- *  Similar in spirit to the 'tee' program.
- *  The header file name is in extra.
+/** RISC OS change here, instead of teeing
+ *  for a header file, write the start of two files
+ *  The default is for just the c file output.
  *  @return 0 (zero) on success, and -1 on failure.
  */
 int filter_tee_header (struct filter *chain)
-{
-	/* This function reads from stdin and writes to both the C file and the
-	 * header file at the same time.
-	 */
-
-	char    buf[512];
-	int     to_cfd = -1;
-	FILE   *to_c = NULL, *to_h = NULL;
-	bool    write_header;
-
-	write_header = (chain->extra != NULL);
-
-	/* Store a copy of the stdout pipe, which is already piped to C file
-	 * through the running chain. Then create a new pipe to the H file as
-	 * stdout, and fork the rest of the chain again.
-	 */
-
-	if ((to_cfd = dup (1)) == -1)
-		flexfatal (_("dup(1) failed"));
-	to_c = fdopen (to_cfd, "w");
-
-	if (write_header) {
-		if (freopen ((char *) chain->extra, "w", stdout) == NULL)
-			flexfatal (_("freopen(headerfilename) failed"));
-
-		filter_apply_chain (chain->next);
-		to_h = stdout;
-	}
-
-	/* Now to_c is a pipe to the C branch, and to_h is a pipe to the H branch.
-	 */
-
-	if (write_header) {
-        fputs (check_4_gnu_m4, to_h);
-		fputs ("m4_changecom`'m4_dnl\n", to_h);
-		fputs ("m4_changequote`'m4_dnl\n", to_h);
-		fputs ("m4_changequote([[,]])[[]]m4_dnl\n", to_h);
-	    fputs ("m4_define([[M4_YY_NOOP]])[[]]m4_dnl\n", to_h);
-		fputs ("m4_define( [[M4_YY_IN_HEADER]],[[]])m4_dnl\n",
-		       to_h);
-		fprintf (to_h, "#ifndef %sHEADER_H\n", prefix);
-		fprintf (to_h, "#define %sHEADER_H 1\n", prefix);
-		fprintf (to_h, "#define %sIN_HEADER 1\n\n", prefix);
-		fprintf (to_h,
-			 "m4_define( [[M4_YY_OUTFILE_NAME]],[[%s]])m4_dnl\n",
-			 headerfilename ? headerfilename : "<stdout>");
-
-	}
-
-    fputs (check_4_gnu_m4, to_c);
-	fputs ("m4_changecom`'m4_dnl\n", to_c);
-	fputs ("m4_changequote`'m4_dnl\n", to_c);
-	fputs ("m4_changequote([[,]])[[]]m4_dnl\n", to_c);
-	fputs ("m4_define([[M4_YY_NOOP]])[[]]m4_dnl\n", to_c);
-	fprintf (to_c, "m4_define( [[M4_YY_OUTFILE_NAME]],[[%s]])m4_dnl\n",
-		 outfilename ? outfilename : "<stdout>");
-
-	while (fgets (buf, sizeof buf, stdin)) {
-		fputs (buf, to_c);
-		if (write_header)
-			fputs (buf, to_h);
-	}
-
-	if (write_header) {
-		fprintf (to_h, "\n");
-
-		/* write a fake line number. It will get fixed by the linedir filter. */
-		if (gen_line_dirs)
-			fprintf (to_h, "#line 4000 \"M4_YY_OUTFILE_NAME\"\n");
-
-		fprintf (to_h, "#undef %sIN_HEADER\n", prefix);
-		fprintf (to_h, "#endif /* %sHEADER_H */\n", prefix);
-		fputs ("m4_undefine( [[M4_YY_IN_HEADER]])m4_dnl\n", to_h);
-
-		fflush (to_h);
-		if (ferror (to_h))
-			lerr (_("error writing output file %s"),
-				(char *) chain->extra);
-
-		else if (fclose (to_h))
-			lerr (_("error closing output file %s"),
-				(char *) chain->extra);
-	}
-
-	fflush (to_c);
-	if (ferror (to_c))
-		lerr (_("error writing output file %s"),
-			outfilename ? outfilename : "<stdout>");
-
-	else if (fclose (to_c))
-		lerr (_("error closing output file %s"),
-			outfilename ? outfilename : "<stdout>");
-
-	while (wait (0) > 0) ;
-
-	FLEX_EXIT (0);
+{ 
+	/* c file opened and closed in main.c  */
+	if( fseek(flout,0,SEEK_CUR) == 0){
+        fputs (check_4_gnu_m4, flout);
+	fputs ("m4_changecom`'m4_dnl\n", flout);
+	fputs ("m4_changequote`'m4_dnl\n", flout);
+	fputs ("m4_changequote([[,]])[[]]m4_dnl\n", flout);
+	fputs ("m4_define([[M4_YY_NOOP]])[[]]m4_dnl\n", flout);
+	/* overwriting for a headerfile begins here */
+	fprintf (flout, "m4_define( [[M4_YY_OUTFILE_NAME]],[[%s]])m4_dnl\n",
+ 		 outfilename ? outfilename : "<stdout>");
+ 	/* create extra space to add extra headerfile lines later */  	 
+ 		      if (headerfilename) {
+	fprintf (flout,"m4_dnl reserve some space for %s          \n", headerfilename);
+	fputs ("m4_dnl prints no output to c file                          \n", flout);
+				}	 
+			}
+		/* file gas been closed after c file use */
+	else if (headerfilename && !h_out) {
+			flout = fopen (flo, "r+");
+			if (flout == NULL)
+		    lerr (_("hdr: error re-opening temp file %s"),flo);
+		    /* overwrite temp file beginning at pos 248 */
+		    if(fseek (flout,248,SEEK_SET) !=0)
+	           lerr (_("error with initial seek_set in header temp %ld\n"));
+				
+		fputs ("m4_define( [[M4_YY_IN_HEADER]],[[]])m4_dnl\n", flout);
+		fprintf (flout, "#ifndef %sHEADER_H\n", prefix);
+		fprintf (flout, "#define %sHEADER_H 1\n", prefix);
+		fprintf (flout, "#define %sIN_HEADER 1\n\n", prefix);
+		fprintf (flout, "m4_define( [[M4_YY_OUTFILE_NAME]],[[%s]])m4_dnl\n",
+ 			 headerfilename ? headerfilename : "<stdout>");
+ 			 	if (fseek (flout,0,SEEK_END) !=0)
+	                lerr (_("error with seek_end of header temp %S"),flout);     
+		    	/* append header file ending */
+		fprintf (flout, "\n#line 4000 \"M4_YY_OUTFILE_NAME\"\n");
+		fprintf (flout, "#undef %sIN_HEADER\n", prefix);
+		fprintf (flout, "#endif /* %sHEADER_H */\n", prefix);
+		fputs ("m4_undefine( [[M4_YY_IN_HEADER]])m4_dnl\n", flout);
+
+	    if (fclose (flout) != 0)
+		    lerr(_("error closing h temp file %s"),flo);
+		
+		}
 	return 0;
 }
 
@@ -337,16 +260,40 @@
  */
 int filter_fix_linedirs (struct filter *chain)
 {
-	char   buf[512];
-	const size_t readsz = sizeof buf;
+	char    *buf = malloc(512);
+	const size_t readsz = 512;
 	int     lineno = 1;
 	bool    in_gen = true;	/* in generated code */
 	bool    last_was_blank = false;
 
+    extern char *m4o;
+    
 	if (!chain)
 		return 0;
+		
+	 FILE *flfn;
 
-	while (fgets (buf, (int) readsz, stdin)) {
+	if (headerfilename && h_out==true) 
+	outfilename = headerfilename;
+
+	if (use_stdout)
+	flfn = stdout;
+	else {
+		if (outfilename) {
+		remove (outfilename);
+	flfn = fopen (outfilename, "w");
+	if (flfn == NULL)
+	lerr (_("flex out: error opening output file %s"),
+ 			outfilename );
+ 			setlinebuf(flfn);
+		}
+	}
+	
+	FILE *m4fn = fopen (m4o, "r");
+	     if (m4fn == NULL)
+	     lerr (_("flex out: error opening m4 file %s"),m4o);
+		
+	while (fgets (buf, (int) readsz, m4fn)) {
 
 		regmatch_t m[10];
 
@@ -412,18 +359,17 @@
 			last_was_blank = false;
 		}
 
-		fputs (buf, stdout);
+		fputs (buf, flfn);
 		lineno++;
 	}
-	fflush (stdout);
-	if (ferror (stdout))
-		lerr (_("error writing output file %s"),
-			outfilename ? outfilename : "<stdout>");
-
-	else if (fclose (stdout))
-		lerr (_("error closing output file %s"),
-			outfilename ? outfilename : "<stdout>");
-
+            free(buf);
+ 		if (fclose (m4fn) != 0)
+		    lerr (_("flex out: error closing m4 file %s"),m4o);
+  		if (fclose (flfn) != 0)
+		    lerr (_("flex out: error closing output %s"),
+		    outfilename ? outfilename : "<stdout>");
+		h_out = true;
+		
 	return 0;
 }
 
