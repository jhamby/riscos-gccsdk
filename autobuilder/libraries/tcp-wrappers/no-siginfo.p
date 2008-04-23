--- shell_cmd.c.orig	2008-04-22 21:36:18.000000000 -0700
+++ shell_cmd.c	2008-04-22 21:38:08.000000000 -0700
@@ -42,17 +42,20 @@
  */
 volatile static int foreign_sigchld;
 volatile static int our_child_pid;
+#if 0
 static void sigchld(int sig, siginfo_t *si, void *unused)
 {
     if (si && si->si_pid != our_child_pid)
 	foreign_sigchld = 1;
 }
+#endif
 
 /* shell_cmd - execute shell command */
 
 void    shell_cmd(command)
 char   *command;
 {
+#if 0
     int     child_pid;
 
     struct sigaction new_action, old_action;
@@ -103,6 +106,7 @@
      * mask and handler. */
     if (foreign_sigchld)
 	raise(SIGCHLD);
+#endif
 }
 
 /* do_child - exec command with { stdin, stdout, stderr } to /dev/null */
