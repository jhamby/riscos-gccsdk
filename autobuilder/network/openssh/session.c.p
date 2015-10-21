--- ./session.c.orig	2015-09-23 16:04:35.157552337 +0200
+++ ./session.c	2015-09-23 16:04:35.173552337 +0200
@@ -1321,7 +1321,7 @@
 
 	/* read $HOME/.ssh/environment. */
 	if (options.permit_user_env && !options.use_login) {
-		snprintf(buf, sizeof buf, "%.200s/.ssh/environment",
+		snprintf(buf, sizeof buf, "%.200s/ssh/environment",
 		    strcmp(pw->pw_dir, "/") ? pw->pw_dir : "");
 		read_environment_file(&env, &envsize, buf);
 	}
@@ -1821,22 +1821,9 @@
 		fflush(NULL);
 		exit(1);
 	} else if (s->is_subsystem == SUBSYSTEM_INT_SFTP) {
-		extern int optind, optreset;
-		int i;
-		char *p, *args;
-
-		setproctitle("%s@%s", s->pw->pw_name, INTERNAL_SFTP_NAME);
-		args = xstrdup(command ? command : "sftp-server");
-		for (i = 0, (p = strtok(args, " ")); p; (p = strtok(NULL, " ")))
-			if (i < ARGV_MAX - 1)
-				argv[i++] = p;
-		argv[i] = NULL;
-		optind = optreset = 1;
-		__progname = argv[0];
-#ifdef WITH_SELINUX
-		ssh_selinux_change_context("sftpd_t");
-#endif
-		exit(sftp_server_main(i, argv, s->pw));
+		printf("Sorry, sftp server capabilities not implemented.\n");
+		fflush(NULL);
+		exit(1);
 	}
 
 	fflush(NULL);
