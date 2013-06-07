--- execute_cmd.c.orig	2013-06-06 17:35:33.362262584 +0100
+++ execute_cmd.c	2013-06-06 17:41:35.653514047 +0100
@@ -2101,6 +2101,9 @@
   COMMAND *cmd;
   struct fd_bitmap *fd_bitmap;
   pid_t lastpid;
+#ifndef JOB_CONTROL
+  int job_control = 0;
+#endif
 
 #if defined (JOB_CONTROL)
   sigset_t set, oset;
