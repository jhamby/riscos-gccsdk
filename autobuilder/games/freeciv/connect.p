--- client/connectdlg_common.c.orig	2015-08-15 11:10:33.000000000 +0000
+++ client/connectdlg_common.c	2021-03-07 10:03:05.516555283 +0000
@@ -19,6 +19,7 @@
 #include <signal.h>             /* SIGTERM and kill */
 #include <string.h>
 #include <time.h>
+#include <swis.h>
 
 #ifdef WIN32_NATIVE
 #include <windows.h>
@@ -101,6 +102,13 @@
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
@@ -229,7 +237,24 @@
     return FALSE;
   }
 
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
   {
     int argc = 0;
     const int max_nargs = 18;
@@ -427,8 +452,10 @@
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
@@ -524,7 +551,11 @@
 
     /* get the full filename path */
     interpret_tilde(challenge_fullname, sizeof(challenge_fullname),
+#ifdef __riscos__
+                    "/<Choices$Write>/Freeciv/");
+#else
 		    "~/.freeciv/");
+#endif
     make_dir(challenge_fullname);
 
     sz_strlcat(challenge_fullname, filename);
