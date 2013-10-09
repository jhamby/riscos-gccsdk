--- gio/gunixfdmessage.c.orig	2013-08-07 15:34:32.000000000 +0100
+++ gio/gunixfdmessage.c	2013-10-09 12:21:28.165944500 +0100
@@ -73,7 +73,7 @@
 static int
 g_unix_fd_message_get_msg_type (GSocketControlMessage *message)
 {
-  return SCM_RIGHTS;
+  return 1;
 }
 
 static GSocketControlMessage *
@@ -88,7 +88,7 @@
   gint *fds;
 
   if (level != SOL_SOCKET ||
-      type != SCM_RIGHTS)
+      type != 1 /*SCM_RIGHTS*/)
     return NULL;
   
   if (size % 4 > 0)
