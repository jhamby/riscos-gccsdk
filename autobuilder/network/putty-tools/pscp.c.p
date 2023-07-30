--- pscp.c.orig	2022-08-21 10:03:28.418580779 +0100
+++ pscp.c	2022-08-21 10:06:37.644642794 +0100
@@ -2147,7 +2147,7 @@
         if (*user == '\0')
             user = NULL;
     }
-
+#ifndef __riscos__
     cmd = snewn(4 * strlen(src) + 100, char);
     strcpy(cmd, "ls -la '");
     p = cmd + strlen(cmd);
@@ -2166,7 +2166,7 @@
 
     do_cmd(host, user, cmd);
     sfree(cmd);
-
+#endif
     if (using_sftp) {
         scp_sftp_listdir(src);
     } else {
