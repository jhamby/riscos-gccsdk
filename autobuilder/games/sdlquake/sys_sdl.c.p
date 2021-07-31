--- sys_sdl.c.orig	2012-04-14 05:30:49.000000000 +0100
+++ sys_sdl.c	2012-04-14 05:24:01.000000000 +0100
@@ -12,8 +12,10 @@
 #include <ctype.h>
 #include <errno.h>
 #ifndef __WIN32__
+#ifndef __riscos__
 #include <sys/ipc.h>
 #include <sys/shm.h>
+#endif
 #include <sys/stat.h>
 #include <sys/wait.h>
 #include <sys/mman.h>
@@ -27,7 +27,7 @@

 int noconinput = 0;

-char *basedir = ".";
+char *basedir = "<SDLQuake$Dir>";
-char *cachedir = "/tmp";
+char *cachedir = "";

 cvar_t  sys_linerefresh = {"sys_linerefresh","0"};// set for entity display
@@ -374,6 +376,8 @@
 	extern int recording;
 	static int frame;

+	int j;
+
 	moncontrol(0);

 //	signal(SIGFPE, floating_point_exception_handler);
@@ -383,7 +387,17 @@
         parms.argc = com_argc;
         parms.argv = com_argv;

-	parms.memsize = 8*1024*1024;
+        j = COM_CheckParm("-mem");
+        if (j) {
+                parms.memsize = (int) (Q_atof(com_argv[j+1]) * 1024 * 1024);
+                //printf("Overriding Memory Size\n");
+        }
+        else {
+                //printf("No need to override?\n");
+                parms.memsize = 8*1024*1024;
+        }
+
+//	parms.memsize = 8*1024*1024;
 	parms.membase = malloc (parms.memsize);
 	parms.basedir = basedir;
 	parms.cachedir = cachedir;
