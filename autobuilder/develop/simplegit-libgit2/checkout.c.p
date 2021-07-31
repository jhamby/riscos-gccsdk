--- src/builtin/checkout.c.orig	2018-11-09 21:16:53.979452667 +0000
+++ src/builtin/checkout.c	2019-05-25 14:03:24.681041761 +0100
@@ -2,6 +2,7 @@
 
 #include <stdlib.h>
 #include <stdio.h>
+#include <stdbool.h>
 #include <string.h>
 #include <git2.h>
 #include "errors.h"
@@ -18,18 +19,47 @@
 	return 1;
 }
 
+static bool try_create_local_from_remote(git_repository *repo,const char *branch)
+{
+	int err;
+	git_reference *branch_ref = NULL;
+	if (git_branch_lookup(&branch_ref,repo,branch,GIT_BRANCH_LOCAL) == 0)
+	{
+		/* Already have a local branch with that name */
+		git_reference_free(branch_ref);
+		return false;
+	}
+	/* Try remote branch */
+	git_reference *remote_branch_ref;
+	char remote_buf[256];
+	snprintf(remote_buf,sizeof(remote_buf),"%s/%s","origin",branch);
+	if (git_branch_lookup(&remote_branch_ref, repo, remote_buf, GIT_BRANCH_REMOTE) == 0)
+	{
+		/* Exists, now try to create a local one from that reference */
+		err = git2_create_branch_from_ref(&branch_ref,remote_branch_ref,repo,branch);
+		git_reference_free(remote_branch_ref);
+		if (err != 0)
+		{
+			fprintf(stderr,"Error code %d\n",err);
+			libgit_error();
+			return false;
+		}
+		return true;
+	}
+	return false;
+}
+
 int cmd_checkout(git_repository *repo, int argc, char **argv)
 {
-	int i, err, rc;
-	char *branch;
-	git_reference *branch_ref;
+	int i, err;
+	int rc = EXIT_FAILURE;
+	char *branch = NULL;
+	git_reference *branch_ref = NULL;
+	git_object *treeish = NULL;
 	git_checkout_options checkout_opts;
 
 	git_strarray paths = {0};
 
-	branch = NULL;
-	rc = EXIT_FAILURE;
-
 	for (i=1;i<argc;i++)
 	{
 		if (argv[i][0] != '-')
@@ -43,47 +73,24 @@
 	if (!branch)
 	{
 		fprintf (stderr, "USAGE: %s <branch>\n", argv[0]);
+		fprintf (stderr, "       %s <commit>\n", argv[0]);
 		fprintf (stderr, "       %s <path>\n", argv[0]);
 		return -1;
 	}
 
-	/* Try local branch */
-	if (git_branch_lookup(&branch_ref,repo,branch,GIT_BRANCH_LOCAL) != 0)
+	if (git_revparse_single(&treeish,repo,branch) != 0)
 	{
-		/* No, try remote branch */
-		git_reference *remote_branch_ref;
-		char remote_buf[256];
-		snprintf(remote_buf,sizeof(remote_buf),"%s/%s","origin",branch);
-		if (git_branch_lookup(&remote_branch_ref, repo, remote_buf, GIT_BRANCH_REMOTE) == 0)
-		{
-			/* Exists, now try to create a local one from that reference */
-			if ((err = git2_create_branch_from_ref(&branch_ref,remote_branch_ref,repo,branch)) != 0)
-			{
-				fprintf(stderr,"Error code %d\n",err);
-				libgit_error();
-				goto out;
-			}
-			git_reference_free(remote_branch_ref);
-		} else
+		/* Attempt to auto-create local branch from remote branch */
+		if (try_create_local_from_remote(repo,branch))
 		{
-			branch_ref = NULL;
+			git_revparse_single(&treeish,repo,branch);
 		}
 	}
-
-	if ((err = git_repository_set_head(repo,branch_ref?git_reference_name(branch_ref):branch)) != 0)
+	if (treeish == NULL)
 	{
-		if (err == GIT_EINVALIDSPEC || err == GIT_ENOTFOUND)
-		{
-			paths.count = 1;
-			paths.strings = &branch;
-			goto cont;
-		}
-
-		fprintf(stderr,"Error code %d\n",err);
-		libgit_error();
-		goto out;
+		paths.count = 1;
+		paths.strings = &branch;
 	}
-cont:
 
 	/* Default options. Note by default we perform a dry checkout */
 	memset(&checkout_opts,0,sizeof(checkout_opts));
@@ -96,15 +103,47 @@
 
 	checkout_opts.checkout_strategy = GIT_CHECKOUT_FORCE|GIT_CHECKOUT_REMOVE_UNTRACKED;//GIT_CHECKOUT_SAFE|GIT_CHECKOUT_UPDATE_UNTRACKED;
 
-	printf("Checking out %s\n",branch_ref?git_reference_name(branch_ref):branch);
-	if (git_checkout_head(repo,&checkout_opts) != 0)
+	printf("Checking out %s%s\n",(paths.count ? "path " : ""), branch);
+	if (git_checkout_tree(repo,treeish,&checkout_opts) != 0)
 	{
 		libgit_error();
 		goto out;
 	}
 
+	/* Now update HEAD to point to the new target */
+	if (treeish != NULL)
+	{
+		/* Try local branch */
+		if (git_branch_lookup(&branch_ref,repo,branch,GIT_BRANCH_LOCAL) != 0)
+		{
+			/* Try commit */
+			const git_oid *id;
+			branch_ref = NULL;
+			id = git_object_id(treeish);
+			if (id != NULL)
+			{
+				if (git_repository_set_head_detached(repo,id) == 0)
+				{
+					goto cont;
+				}
+			}
+		}
+
+		if ((err = git_repository_set_head(repo,branch_ref?git_reference_name(branch_ref):branch)) != 0)
+		{
+			fprintf(stderr,"Error code %d\n",err);
+			libgit_error();
+			goto out;
+		}
+	}
+
+cont:
 	rc = EXIT_SUCCESS;
+
 out:
+	if (treeish)
+		git_object_free(treeish);
+
 	if (branch_ref)
 		git_reference_free(branch_ref);
 
