--- src/builtin/status.c.orig	2018-11-09 21:16:53.995452345 +0000
+++ src/builtin/status.c	2019-05-25 14:05:34.475752666 +0100
@@ -108,6 +108,9 @@
 			{
 				printf("# On branch %s\n",branch_name);
 			}
+		} else if (git_repository_head_detached(repo))
+		{
+			printf("# HEAD is detached at %s\n", git_oid_tostr_s(git_reference_target(head_ref)));
 		}
 	} else
 	{
@@ -144,7 +147,7 @@
 				printf("# Your branch is ahead its upstream branch '%s' by %d commits.\n", upstream_name, (int)ahead);
 			}
 		}
-	} else if (err != GIT_ENOTFOUND)
+	} else if ((err != GIT_ENOTFOUND) && !git_repository_head_detached(repo))
 	{
 		goto out;
 	}
