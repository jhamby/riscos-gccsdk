--- gio/gunixfdmessage.c.orig	2009-09-28 14:53:03.000000000 -0700
+++ gio/gunixfdmessage.c	2009-09-28 14:54:50.000000000 -0700
@@ -69,7 +69,7 @@
 static int
 g_unix_fd_message_get_msg_type (GSocketControlMessage *message)
 {
-  return SCM_RIGHTS;
+  return 1;
 }
 
 static GSocketControlMessage *
@@ -81,7 +81,7 @@
   GUnixFDMessage *message;
 
   if (level != SOL_SOCKET ||
-      type != SCM_RIGHTS)
+      type != 1 /*SCM_RIGHTS*/)
     return NULL;
   
   if (size % 4 > 0)
