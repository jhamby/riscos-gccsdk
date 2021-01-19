--- ssh-keygen.c.orig	2014-07-03 12:24:41.000000000 +0100
+++ ssh-keygen.c	2021-01-19 18:34:04.294135519 +0000
@@ -245,7 +245,7 @@
 			break;
 		}
 	}
-	snprintf(identity_file, sizeof(identity_file), "%s/%s", pw->pw_dir, name);
+	snprintf(identity_file, sizeof(identity_file), "%s.%s", pw->pw_dir, name);
 	fprintf(stderr, "%s (%s): ", prompt, identity_file);
 	if (fgets(buf, sizeof(buf), stdin) == NULL)
 		exit(1);
@@ -955,7 +955,7 @@
 			continue;
 		}
 		key_free(private);
-		strlcat(identity_file, ".pub", sizeof(identity_file));
+		strlcat(identity_file, "/pub", sizeof(identity_file));
 		fd = open(identity_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
 		if (fd == -1) {
 			printf("Could not save your public key in %s\n",
@@ -1442,7 +1442,7 @@
 	public = key_from_private(private);
 	key_free(private);
 
-	strlcat(identity_file, ".pub", sizeof(identity_file));
+	strlcat(identity_file, "/pub", sizeof(identity_file));
 	fd = open(identity_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
 	if (fd == -1) {
 		printf("Could not save your public key in %s\n", identity_file);
@@ -2604,7 +2604,7 @@
 		ask_filename(pw, "Enter file in which to save the key");
 
 	/* Create ~/.ssh directory if it doesn't already exist. */
-	snprintf(dotsshdir, sizeof dotsshdir, "%s/%s",
+	snprintf(dotsshdir, sizeof dotsshdir, "%s.%s",
 	    pw->pw_dir, _PATH_SSH_USER_DIR);
 	if (strstr(identity_file, dotsshdir) != NULL) {
 		if (stat(dotsshdir, &st) < 0) {
@@ -2683,7 +2683,7 @@
 	if (!quiet)
 		printf("Your identification has been saved in %s.\n", identity_file);
 
-	strlcat(identity_file, ".pub", sizeof(identity_file));
+	strlcat(identity_file, "/pub", sizeof(identity_file));
 	fd = open(identity_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
 	if (fd == -1) {
 		printf("Could not save your public key in %s\n", identity_file);
