--- client/connectdlg_common.c.orig	2011-07-30 20:33:31.000000000 +0100
+++ client/connectdlg_common.c	2011-07-30 20:34:57.000000000 +0100
@@ -19,6 +19,7 @@
 #include <signal.h>             /* SIGTERM and kill */
 #include <string.h>
 #include <time.h>
+#include <swis.h>
 
 #ifdef WIN32_NATIVE
 #include <windows.h>
@@ -100,6 +101,13 @@
          the game.
 **************************************************************************/ 
 
+# ifdef __riscos__
+_kernel_oserror *SWI_OS_CLI(const char *command)
+{ /* execute a command */
+  return _swix(OS_CLI,_IN(0),command);
+}
+#endif
+
 /************************************************************************** 
 Tests if the client has started the server.
 **************************************************************************/ 
@@ -208,7 +216,24 @@
   /* find a free port */ 
   internal_server_port = find_next_free_port(DEFAULT_SOCK_PORT);
 
-# ifdef HAVE_WORKING_FORK
+# ifdef __riscos__
+  {
+
+    char command[256];
+    char script[100];
+
+    if (scriptfile) {
+      fc_snprintf(script, sizeof(script), " --read %s", scriptfile);
+    } else {
+      *script = '\0';
+    }
+
+    fc_snprintf(command, sizeof(command), "StartDesktopTask <CivServer$Dir> -p %d -q 1 -e --saves /<Choices$Write>/Freeciv/saves %s", internal_server_port, script);
+
+    if (SWI_OS_CLI(command)) return FALSE;
+  }
+
+# elif HAVE_WORKING_FORK
   server_pid = fork();
   
   if (server_pid == 0) {
@@ -356,8 +381,10 @@
   while (connect_to_server(user_name, "localhost", internal_server_port, 
                            buf, sizeof(buf)) == -1) {
     fc_usleep(WAIT_BETWEEN_TRIES);
-#ifdef HAVE_WORKING_FORK
-#ifndef WIN32_NATIVE
+#ifdef __riscos__
+    SDL_PumpEvents();
+#elif HAVE_WORKING_FORK
+#if !defined(WIN32_NATIVE)
     if (waitpid(server_pid, NULL, WNOHANG) != 0) {
       break;
     }
@@ -469,7 +496,11 @@
 
     /* get the full filename path */
     interpret_tilde(challenge_fullname, sizeof(challenge_fullname),
+#ifdef __riscos__
+                    "/<Choices$Write>/Freeciv/");
+#else
 		    "~/.freeciv/");
+#endif
     make_dir(challenge_fullname);
 
     sz_strlcat(challenge_fullname, filename);
