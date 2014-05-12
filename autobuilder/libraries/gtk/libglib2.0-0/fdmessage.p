--- gio/gunixfdmessage.c.orig	2013-11-07 15:29:13.000000000 +0000
+++ gio/gunixfdmessage.c	2014-01-06 18:15:02.330386626 +0000
@@ -70,7 +70,7 @@
 static int
 g_unix_fd_message_get_msg_type (GSocketControlMessage *message)
 {
-  return SCM_RIGHTS;
+  return 1;
 }
 
 static GSocketControlMessage *
@@ -85,7 +85,7 @@
   gint *fds;
 
   if (level != SOL_SOCKET ||
-      type != SCM_RIGHTS)
+      type != 1 /*SCM_RIGHTS*/)
     return NULL;
   
   if (size % 4 > 0)
