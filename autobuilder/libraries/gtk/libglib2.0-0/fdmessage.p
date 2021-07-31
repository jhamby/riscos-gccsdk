--- gio/gunixfdmessage.c.orig	2017-02-13 15:15:29.000000000 +0000
+++ gio/gunixfdmessage.c	2017-04-01 20:02:42.484064630 +0100
@@ -77,7 +77,7 @@
 static int
 g_unix_fd_message_get_msg_type (GSocketControlMessage *message)
 {
-  return SCM_RIGHTS;
+  return 1;
 }
 
 static GSocketControlMessage *
@@ -92,7 +92,7 @@
   gint *fds;
 
   if (level != SOL_SOCKET ||
-      type != SCM_RIGHTS)
+      type != 1 /*SCM_RIGHTS*/)
     return NULL;
   
   if (size % 4 > 0)
