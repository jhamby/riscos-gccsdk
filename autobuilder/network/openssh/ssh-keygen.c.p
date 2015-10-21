--- ./ssh-keygen.c.orig	2015-09-23 16:04:34.941552339 +0200
+++ ./ssh-keygen.c	2015-09-23 16:04:34.957552339 +0200
@@ -263,7 +263,7 @@
 		}
 	}
 	snprintf(identity_file, sizeof(identity_file),
-	    "%s/%s", pw->pw_dir, name);
+	    "%s.%s", pw->pw_dir, name);
 	printf("%s (%s): ", prompt, identity_file);
 	fflush(stdout);
 	if (fgets(buf, sizeof(buf), stdin) == NULL)
@@ -2607,7 +2607,7 @@
 		ask_filename(pw, "Enter file in which to save the key");
 
 	/* Create ~/.ssh directory if it doesn't already exist. */
-	snprintf(dotsshdir, sizeof dotsshdir, "%s/%s",
+	snprintf(dotsshdir, sizeof dotsshdir, "%s.%s",
 	    pw->pw_dir, _PATH_SSH_USER_DIR);
 	if (strstr(identity_file, dotsshdir) != NULL) {
 		if (stat(dotsshdir, &st) < 0) {
@@ -2687,7 +2687,7 @@
 	if (!quiet)
 		printf("Your identification has been saved in %s.\n", identity_file);
 
-	strlcat(identity_file, ".pub", sizeof(identity_file));
+	strlcat(identity_file, "/pub", sizeof(identity_file));
 	if ((fd = open(identity_file, O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1)
 		fatal("Unable to save public key to %s: %s",
 		    identity_file, strerror(errno));
