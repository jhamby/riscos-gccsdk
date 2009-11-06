--- client/connectdlg_common.c.orig	2009-11-05 21:23:48.000000000 -0800
+++ client/connectdlg_common.c	2009-11-06 09:46:20.000000000 -0800
@@ -215,7 +215,25 @@
   /* find a free port */ 
   internal_server_port = find_next_free_port(DEFAULT_SOCK_PORT);
 
-# ifdef HAVE_WORKING_FORK
+# ifdef __riscos__
+  {
+    extern void *__os_cli(const char *command);
+
+    char command[256];
+    char script[100];
+
+    if (scriptfile) {
+      my_snprintf(script, sizeof(script), " --read %s", scriptfile);
+    } else {
+      *script = '\0';
+    }
+
+    my_snprintf(command, sizeof(command), "StartDesktopTask <CivServer$Dir> -p %d -q 1 -e --saves /<Choices$Write>/Freeciv/saves %s", internal_server_port, script);
+
+    if (__os_cli(command)) return FALSE;
+  }
+
+# elif HAVE_WORKING_FORK
   server_pid = fork();
   
   if (server_pid == 0) {
@@ -343,8 +361,10 @@
   while (connect_to_server(user_name, "localhost", internal_server_port, 
                            buf, sizeof(buf)) == -1) {
     myusleep(WAIT_BETWEEN_TRIES);
-#ifdef HAVE_WORKING_FORK
-#ifndef WIN32_NATIVE
+#ifdef __riscos__
+    SDL_PumpEvents();
+#elif HAVE_WORKING_FORK
+#if !defined(WIN32_NATIVE)
     if (waitpid(server_pid, NULL, WNOHANG) != 0) {
       break;
     }
