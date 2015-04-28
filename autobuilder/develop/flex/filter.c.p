--- filter.c.orig	2014-03-27 01:46:44.000000000 +1300
+++ filter.c	2015-04-18 13:08:44.433791971 +1200
@@ -26,6 +26,9 @@
     "m4_dnl ifdef(`__gnu__', ,"
     "`errprint(Flex requires GNU M4. Set the PATH or set the M4 environment variable to its path name.)"
     " m4exit(2)')\n";
+int pass = -1;
+extern FILE *flout; 
+extern FILE *flhout;
 
 
 /** global chain. */
@@ -65,7 +68,7 @@
 
 
 	/* allocate argv, and populate it with the argument list. */
-	max_args = 8;
+	max_args = 11;
 	f->argv =
 		(const char **) flex_alloc (sizeof (char *) *
 					    (max_args + 1));
@@ -128,17 +131,19 @@
 	return f;
 }
 
+/* a chain emptying filter so the fork/exec of m4 doesn't run unwanted filters */
+int filter_empty (struct filter *chain)
+{
+return 0;
+}
+
 /** Fork and exec entire filter chain.
  *  @param chain The head of the chain.
  *  @return true on success.
  */
 bool filter_apply_chain (struct filter * chain)
 {
-	int     pid, pipes[2];
-	int     r;
-	const int readsz = 512;
-	char   *buf;
-
+    pid_t pid;
 
 	/* Tricky recursion, since we want to begin the chain
 	 * at the END. Why? Because we need all the forked processes
@@ -152,32 +157,12 @@
 	/* Now we are the right-most unprocessed link in the chain.
 	 */
 
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
-
 		/* run as a filter, either internally or by exec */
 		if (chain->filter_func) {
 			int     r;
@@ -197,11 +182,6 @@
 	}
 
 	/* Parent */
-	close (pipes[0]);
-	if (dup2 (pipes[1], fileno (stdout)) == -1)
-		flexfatal (_("dup2(pipes[1],1)"));
-	close (pipes[1]);
-    fseek (stdout, 0, SEEK_CUR);
 
 	return true;
 }
@@ -227,111 +207,37 @@
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
 {
-	/* This function reads from stdin and writes to both the C file and the
-	 * header file at the same time.
-	 */
-
-	const int readsz = 512;
-	char   *buf;
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
+    bool    write_header;
+    write_header = (chain->extra != NULL);
 
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
+    fputs (check_4_gnu_m4, flout);
+	fputs ("m4_changecom`'m4_dnl\n", flout);
+	fputs ("m4_changequote`'m4_dnl\n", flout);
+	fputs ("m4_changequote([[,]])[[]]m4_dnl\n", flout);
+	fputs ("m4_define([[M4_YY_NOOP]])[[]]m4_dnl\n", flout);
+	fprintf (flout, "m4_define( [[M4_YY_OUTFILE_NAME]],[[%s]])m4_dnl\n",
 		 outfilename ? outfilename : "<stdout>");
 
-	buf = (char *) flex_alloc (readsz);
-	if (!buf)
-		flexerror (_("flex_alloc failed in filter_tee_header"));
-	while (fgets (buf, readsz, stdin)) {
-		fputs (buf, to_c);
-		if (write_header)
-			fputs (buf, to_h);
-	}
-
-	if (write_header) {
-		fprintf (to_h, "\n");
-
-		/* write a fake line number. It will get fixed by the linedir filter. */
-		fprintf (to_h, "#line 4000 \"M4_YY_OUTFILE_NAME\"\n");
-
-		fprintf (to_h, "#undef %sIN_HEADER\n", prefix);
-		fprintf (to_h, "#endif /* %sHEADER_H */\n", prefix);
-		fputs ("m4_undefine( [[M4_YY_IN_HEADER]])m4_dnl\n", to_h);
-
-		fflush (to_h);
-		if (ferror (to_h))
-			lerrsf (_("error writing output file %s"),
-				(char *) chain->extra);
-
-		else if (fclose (to_h))
-			lerrsf (_("error closing output file %s"),
-				(char *) chain->extra);
-	}
-
-	fflush (to_c);
-	if (ferror (to_c))
-		lerrsf (_("error writing output file %s"),
-			outfilename ? outfilename : "<stdout>");
-
-	else if (fclose (to_c))
-		lerrsf (_("error closing output file %s"),
-			outfilename ? outfilename : "<stdout>");
-
-	while (wait (0) > 0) ;
-
-	exit (0);
+if (write_header) {
+        fputs (check_4_gnu_m4, flhout);
+		fputs ("m4_changecom`'m4_dnl\n", flhout);
+		fputs ("m4_changequote`'m4_dnl\n", flhout);
+		fputs ("m4_changequote([[,]])[[]]m4_dnl\n", flhout);
+		fputs ("m4_define([[M4_YY_NOOP]])[[]]m4_dnl\n", flhout);
+		fputs ("m4_define( [[M4_YY_IN_HEADER]],[[]])m4_dnl\n", flhout);
+		fprintf (flhout, "#ifndef %sHEADER_H\n", prefix);
+		fprintf (flhout, "#define %sHEADER_H 1\n", prefix);
+		fprintf (flhout, "#define %sIN_HEADER 1\n\n", prefix);
+		fprintf (flhout, "m4_define( [[M4_YY_OUTFILE_NAME]],[[%s]])m4_dnl\n",
+			 headerfilename ? headerfilename : "<stdout>");
+        }
 	return 0;
 }
 
@@ -340,6 +246,8 @@
  * can add or remove lines.  This only adjusts line numbers for generated code,
  * not user code. This also happens to be a good place to squeeze multiple
  * blank lines into a single blank line.
+ * RISC OS - this is not added to the the filter chain, it is run as a function
+ * after the m4 output tempfile (m4o) as been received.
  */
 int filter_fix_linedirs (struct filter *chain)
 {
@@ -348,15 +256,28 @@
 	int     lineno = 1;
 	bool    in_gen = true;	/* in generated code */
 	bool    last_was_blank = false;
+    extern char *m4o;
 
 	if (!chain)
 		return 0;
+    pass += 1;
+	FILE *infn = fopen(m4o, "r");
+	FILE *outfn = NULL;
+		char *ldoutfile = ""; 
+	if (headerfilename && pass)
+	ldoutfile = headerfilename;
+	else if(outfilename)
+	ldoutfile = outfilename;
+	if (use_stdout)
+	outfn = stdout;
+	else
+	outfn = fopen( ldoutfile, "w");
 
 	buf = (char *) flex_alloc (readsz);
 	if (!buf)
 		flexerror (_("flex_alloc failed in filter_fix_linedirs"));
 
-	while (fgets (buf, readsz, stdin)) {
+	while (fgets (buf, readsz, infn)) {
 
 		regmatch_t m[10];
 
@@ -426,17 +347,22 @@
 			last_was_blank = false;
 		}
 
-		fputs (buf, stdout);
+		fputs (buf, outfn);
 		lineno++;
 	}
-	fflush (stdout);
-	if (ferror (stdout))
+if (fclose (infn))
+		lerrsf (_("error closing m4output file %s"),m4o);
+if (!use_stdout){
+fsync((int)outfn);
+	if (ferror (outfn))
 		lerrsf (_("error writing output file %s"),
 			outfilename ? outfilename : "<stdout>");
 
-	else if (fclose (stdout))
+	else if (fclose (outfn))
 		lerrsf (_("error closing output file %s"),
 			outfilename ? outfilename : "<stdout>");
+        }
+else fflush(outfn);
 
 	return 0;
 }
