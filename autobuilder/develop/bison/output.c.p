--- src/output.c.orig	2019-02-05 18:26:35.000000000 +1300
+++ src/output.c	2020-09-18 20:54:22.448615770 +1200
@@ -27,10 +27,8 @@
 #include <path-join.h>
 #include <quotearg.h>
 #include <relocatable.h> /* relocate2 */
-#include <spawn-pipe.h>
-#include <timevar.h>
-#include <wait-process.h>
-
+#include <sys/wait.h>
+#include <error.h>
 #include "complain.h"
 #include "files.h"
 #include "getargs.h"
@@ -566,7 +564,7 @@
 output_skeleton (void)
 {
   /* Compute the names of the package data dir and skeleton files.  */
-  char const *m4 = (m4 = getenv ("M4")) ? m4 : M4;
+  char const *m4 = (m4 = getenv ("M4")) ? m4 : "m4";
   char const *datadir = pkgdatadir ();
   char *skeldir = xpath_join (datadir, "skeletons");
   char *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
@@ -579,9 +577,6 @@
      installation.  A faulty installation can cause deadlock, so a
      cheap sanity check is worthwhile.  */
   xfclose (xfopen (m4sugar, "r"));
-
-  /* Create an m4 subprocess connected to us via two pipes.  */
-
   if (trace_flag & trace_tools)
     fprintf (stderr, "running: %s %s - %s %s\n",
              m4, m4sugar, m4bison, skel);
@@ -593,10 +588,13 @@
      See the thread starting at
      <http://lists.gnu.org/archive/html/bug-bison/2008-07/msg00000.html>
      for details.  */
-  int filter_fd[2];
+   FILE *in;
+   FILE *out;
+   char const *tmp_file_1 = mktemp("/tmp/bsXXXXXX");
+   char const *tmp_file_2 = mktemp("/tmp/m4XXXXXX");
   pid_t pid;
   {
-    char const *argv[10];
+    char const *argv[11];
     int i = 0;
     argv[i++] = m4;
 
@@ -611,21 +609,40 @@
        for details.  */
     if (*M4_GNU_OPTION)
       argv[i++] = M4_GNU_OPTION;
-
-    argv[i++] = "-I";
-    argv[i++] = datadir;
     if (trace_flag & trace_m4)
       argv[i++] = "-dV";
-    argv[i++] = m4sugar;
-    argv[i++] = "-";
-    argv[i++] = m4bison;
-    argv[i++] = skel;
-    argv[i++] = NULL;
-    aver (i <= ARRAY_CARDINALITY (argv));
-
-    /* The ugly cast is because gnulib gets the const-ness wrong.  */
-    pid = create_pipe_bidi ("m4", m4, (char **)(void*)argv, false, true,
-                            true, filter_fd);
+      argv[i++] = "-I";
+      argv[i++] = datadir;
+      argv[i++] = m4sugar;
+      argv[i++] = tmp_file_1;   /* argv[i++] = "-"; RISCOS: use tmpfiles */
+      argv[i++] = m4bison;
+      argv[i++] = skel;
+      argv[i++] = ">",
+      argv[i++] = tmp_file_2;  /* RISC OS: redirect m4 output to tmpfile  */                  
+      argv[i++] = NULL;
+    
+     out = xfopen (tmp_file_1, "w");
+       if (! out)
+          error (EXIT_FAILURE, get_errno (),
+          "xfopen");
+    if (trace_flag & trace_muscles)
+       muscles_output (stderr);
+    muscles_output (out);
+    xfclose (out);
+    
+    if ((pid = fork()) == 0) {   /* Child runs user job */
+           if (execve(argv[0], (char **const)argv, environ) < 0) {
+           printf("%s: Command not found.\n", argv[0]);
+        exit(0);
+        }
+    }
+    /* Parent */
+    int child_status;
+    while (wait(&child_status) > 0){
+       if (!WIFEXITED (child_status) || WEXITSTATUS (child_status) != 0)
+         error (EXIT_FAILURE, get_errno (),
+       "bison: child error from m4");     
+		}
   }
 
   free (skeldir);
@@ -633,27 +650,15 @@
   free (m4bison);
   free (skel);
 
-  if (trace_flag & trace_muscles)
-    muscles_output (stderr);
-  {
-    FILE *out = xfdopen (filter_fd[1], "w");
-    muscles_output (out);
-    xfclose (out);
-  }
-
   /* Read and process m4's output.  */
-  timevar_push (tv_m4);
-  {
-    FILE *in = xfdopen (filter_fd[0], "r");
-    scan_skel (in);
-    /* scan_skel should have read all of M4's output.  Otherwise, when we
-       close the pipe, we risk letting M4 report a broken-pipe to the
-       Bison user.  */
-    aver (feof (in));
-    xfclose (in);
-  }
-  wait_subprocess (pid, "m4", false, false, true, true, NULL);
-  timevar_pop (tv_m4);
+   in = xfopen (tmp_file_2, "r");
+   if (! in)
+    error (EXIT_FAILURE, get_errno (),
+       "xfopen");
+   scan_skel (in);
+   xfclose (in);
+   remove (tmp_file_1);
+   remove (tmp_file_2);
 }
 
 static void
