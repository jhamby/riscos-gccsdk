--- src/builtin/cli/submodule_cli.c.orig	2019-05-10 22:48:47.347706684 +0100
+++ src/builtin/cli/submodule_cli.c	2019-05-10 22:47:11.084732652 +0100
@@ -8,6 +8,7 @@
 {
 	int help;
 	int init;
+	int submodule;
 	int update;
 };
 
@@ -15,6 +16,7 @@
 {
 	int help_cmd;
 	int init_pos;
+	int submodule_pos;
 	int update_pos;
 };
 
@@ -30,10 +32,10 @@
 	{
 		return 1;
 	}
-	if (cli->init)
+	if (cli->submodule && cli->init)
 	{
 	}
-	else if (cli->update)
+	else if (cli->submodule && cli->update)
 	{
 	}
 	else
@@ -58,8 +60,8 @@
 		return 0;
 	}
 	fprintf(stderr, "usage: %s <command> [<options>]\n", cmd);
-	fprintf(stderr, "init\n");
-	fprintf(stderr, "update\n");
+	fprintf(stderr, "submodule init\n");
+	fprintf(stderr, "submodule update\n");
 	return 1;
 }
 
@@ -79,13 +81,19 @@
 		{
 			cli->init = 1;
 			aux->init_pos = i;
+			cur_command = 3;
+		}
+		else if (!strcmp(argv[i], "submodule"))
+		{
+			cli->submodule = 1;
+			aux->submodule_pos = i;
 			cur_command = 2;
 		}
 		else if (!strcmp(argv[i], "update"))
 		{
 			cli->update = 1;
 			aux->update_pos = i;
-			cur_command = 3;
+			cur_command = 4;
 		}
 		else
 		{
