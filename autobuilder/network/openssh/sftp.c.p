--- ./sftp.c.orig	2015-09-23 16:04:35.033552338 +0200
+++ ./sftp.c	2015-09-23 16:04:35.049552338 +0200
@@ -140,7 +140,6 @@
 	I_CHGRP,
 	I_CHMOD,
 	I_CHOWN,
-	I_DF,
 	I_GET,
 	I_HELP,
 	I_LCHDIR,
@@ -183,7 +182,6 @@
 	{ "chgrp",	I_CHGRP,	REMOTE	},
 	{ "chmod",	I_CHMOD,	REMOTE	},
 	{ "chown",	I_CHOWN,	REMOTE	},
-	{ "df",		I_DF,		REMOTE	},
 	{ "dir",	I_LS,		REMOTE	},
 	{ "exit",	I_QUIT,		NOARGS	},
 	{ "get",	I_GET,		REMOTE	},
@@ -370,10 +368,10 @@
 parse_getput_flags(const char *cmd, char **argv, int argc,
     int *aflag, int *fflag, int *pflag, int *rflag)
 {
-	extern int opterr, optind, optopt, optreset;
+	extern int opterr, optind, optopt;
 	int ch;
 
-	optind = optreset = 1;
+	optind = 1;
 	opterr = 0;
 
 	*aflag = *fflag = *rflag = *pflag = 0;
@@ -405,10 +403,10 @@
 static int
 parse_link_flags(const char *cmd, char **argv, int argc, int *sflag)
 {
-	extern int opterr, optind, optopt, optreset;
+	extern int opterr, optind, optopt;
 	int ch;
 
-	optind = optreset = 1;
+	optind = 1;
 	opterr = 0;
 
 	*sflag = 0;
@@ -429,10 +427,10 @@
 static int
 parse_rename_flags(const char *cmd, char **argv, int argc, int *lflag)
 {
-	extern int opterr, optind, optopt, optreset;
+	extern int opterr, optind, optopt;
 	int ch;
 
-	optind = optreset = 1;
+	optind = 1;
 	opterr = 0;
 
 	*lflag = 0;
@@ -453,10 +451,10 @@
 static int
 parse_ls_flags(char **argv, int argc, int *lflag)
 {
-	extern int opterr, optind, optopt, optreset;
+	extern int opterr, optind, optopt;
 	int ch;
 
-	optind = optreset = 1;
+	optind = 1;
 	opterr = 0;
 
 	*lflag = LS_NAME_SORT;
@@ -504,39 +502,12 @@
 }
 
 static int
-parse_df_flags(const char *cmd, char **argv, int argc, int *hflag, int *iflag)
-{
-	extern int opterr, optind, optopt, optreset;
-	int ch;
-
-	optind = optreset = 1;
-	opterr = 0;
-
-	*hflag = *iflag = 0;
-	while ((ch = getopt(argc, argv, "hi")) != -1) {
-		switch (ch) {
-		case 'h':
-			*hflag = 1;
-			break;
-		case 'i':
-			*iflag = 1;
-			break;
-		default:
-			error("%s: Invalid flag -%c", cmd, optopt);
-			return -1;
-		}
-	}
-
-	return optind;
-}
-
-static int
 parse_no_flags(const char *cmd, char **argv, int argc)
 {
-	extern int opterr, optind, optopt, optreset;
+	extern int opterr, optind, optopt;
 	int ch;
 
-	optind = optreset = 1;
+	optind = 1;
 	opterr = 0;
 
 	while ((ch = getopt(argc, argv, "")) != -1) {
@@ -948,56 +919,6 @@
 	return 0;
 }
 
-static int
-do_df(struct sftp_conn *conn, char *path, int hflag, int iflag)
-{
-	struct sftp_statvfs st;
-	char s_used[FMT_SCALED_STRSIZE];
-	char s_avail[FMT_SCALED_STRSIZE];
-	char s_root[FMT_SCALED_STRSIZE];
-	char s_total[FMT_SCALED_STRSIZE];
-	unsigned long long ffree;
-
-	if (do_statvfs(conn, path, &st, 1) == -1)
-		return -1;
-	if (iflag) {
-		ffree = st.f_files ? (100 * (st.f_files - st.f_ffree) / st.f_files) : 0;
-		printf("     Inodes        Used       Avail      "
-		    "(root)    %%Capacity\n");
-		printf("%11llu %11llu %11llu %11llu         %3llu%%\n",
-		    (unsigned long long)st.f_files,
-		    (unsigned long long)(st.f_files - st.f_ffree),
-		    (unsigned long long)st.f_favail,
-		    (unsigned long long)st.f_ffree, ffree);
-	} else if (hflag) {
-		strlcpy(s_used, "error", sizeof(s_used));
-		strlcpy(s_avail, "error", sizeof(s_avail));
-		strlcpy(s_root, "error", sizeof(s_root));
-		strlcpy(s_total, "error", sizeof(s_total));
-		fmt_scaled((st.f_blocks - st.f_bfree) * st.f_frsize, s_used);
-		fmt_scaled(st.f_bavail * st.f_frsize, s_avail);
-		fmt_scaled(st.f_bfree * st.f_frsize, s_root);
-		fmt_scaled(st.f_blocks * st.f_frsize, s_total);
-		printf("    Size     Used    Avail   (root)    %%Capacity\n");
-		printf("%7sB %7sB %7sB %7sB         %3llu%%\n",
-		    s_total, s_used, s_avail, s_root,
-		    (unsigned long long)(100 * (st.f_blocks - st.f_bfree) /
-		    st.f_blocks));
-	} else {
-		printf("        Size         Used        Avail       "
-		    "(root)    %%Capacity\n");
-		printf("%12llu %12llu %12llu %12llu         %3llu%%\n",
-		    (unsigned long long)(st.f_frsize * st.f_blocks / 1024),
-		    (unsigned long long)(st.f_frsize *
-		    (st.f_blocks - st.f_bfree) / 1024),
-		    (unsigned long long)(st.f_frsize * st.f_bavail / 1024),
-		    (unsigned long long)(st.f_frsize * st.f_bfree / 1024),
-		    (unsigned long long)(100 * (st.f_blocks - st.f_bfree) /
-		    st.f_blocks));
-	}
-	return 0;
-}
-
 /*
  * Undo escaping of glob sequences in place. Used to undo extra escaping
  * applied in makeargv() when the string is destined for a function that
@@ -1319,18 +1240,6 @@
 		if (cmdnum != I_RM)
 			undo_glob_escape(*path1);
 		break;
-	case I_DF:
-		if ((optidx = parse_df_flags(cmd, argv, argc, hflag,
-		    iflag)) == -1)
-			return -1;
-		/* Default to current directory if no path specified */
-		if (argc - optidx < 1)
-			*path1 = NULL;
-		else {
-			*path1 = xstrdup(argv[optidx]);
-			undo_glob_escape(*path1);
-		}
-		break;
 	case I_LS:
 		if ((optidx = parse_ls_flags(argv, argc, lflag)) == -1)
 			return(-1);
@@ -1514,13 +1423,6 @@
 		path1 = make_absolute(path1, *pwd);
 		err = do_globbed_ls(conn, path1, tmp, lflag);
 		break;
-	case I_DF:
-		/* Default to current directory if no path specified */
-		if (path1 == NULL)
-			path1 = xstrdup(*pwd);
-		path1 = make_absolute(path1, *pwd);
-		err = do_df(conn, path1, hflag, iflag);
-		break;
 	case I_LCHDIR:
 		tmp = tilde_expand_filename(path1, getuid());
 		free(path1);
@@ -2171,7 +2073,7 @@
 #else /* USE_PIPES */
 	int inout[2];
 
-	if (socketpair(AF_UNIX, SOCK_STREAM, 0, inout) == -1)
+	if (socketpair(AF_INET, SOCK_STREAM, 0, inout) == -1)
 		fatal("socketpair: %s", strerror(errno));
 	*in = *out = inout[0];
 	c_in = c_out = inout[1];
