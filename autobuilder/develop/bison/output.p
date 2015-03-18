--- src/output.c.orig	2015-01-19 03:01:16.000000000 +1300
+++ src/output.c	2015-03-12 18:41:05.080384199 +1300
@@ -26,10 +26,9 @@
 #include <filename.h>
 #include <get-errno.h>
 #include <quotearg.h>
-#include <spawn-pipe.h>
 #include <timevar.h>
-#include <wait-process.h>
-
+#include <sys/wait.h>
+#include <error.h>
 #include "complain.h"
 #include "files.h"
 #include "getargs.h"
@@ -518,12 +517,6 @@
   muscle_insert_unsigned_int_table ("conflicting_rules", conflict_list,
                                     0, 1, conflict_list_cnt);
 }
-
-
-/*--------------------------------------------.
-| Output the definitions of all the muscles.  |
-`--------------------------------------------*/
-
 static void
 muscles_output (FILE *out)
 {
@@ -535,7 +528,6 @@
   /* Must be last.  */
   muscles_m4_output (out);
 }
-
 /*---------------------------.
 | Call the skeleton parser.  |
 `---------------------------*/
@@ -543,7 +535,10 @@
 static void
 output_skeleton (void)
 {
-  int filter_fd[2];
+  FILE *in;
+  FILE *out;
+  char const *tmp_file_1 = mktemp("/tmp/bsXXXXXX");
+  char const *tmp_file_2 = mktemp("/tmp/m4XXXXXX");
   pid_t pid;
 
   /* Compute the names of the package data dir and skeleton files.  */
@@ -573,10 +568,10 @@
      See the thread starting at
      <http://lists.gnu.org/archive/html/bug-bison/2008-07/msg00000.html>
      for details.  */
-  {
-    char const *argv[10];
+
+    char const *argv[11];
     int i = 0;
-    argv[i++] = m4;
+       argv[i++] = "m4";  /* riscos - use quotes to stop expanding unix path */
 
     /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
        extensions, which Bison's skeletons depend on.  With older M4,
@@ -595,42 +590,49 @@
     if (trace_flag & trace_m4)
       argv[i++] = "-dV";
     argv[i++] = m4sugar;
-    argv[i++] = "-";
+    argv[i++] = tmp_file_1;       /* argv[i++] = "-"; riscos: use tmpfiles */
     argv[i++] = m4bison;
     argv[i++] = skel;
+    argv[i++] = ">";
+    argv[i++] = tmp_file_2;       /* redirect m4 output to tmpfile  */
     argv[i++] = NULL;
-    aver (i <= ARRAY_CARDINALITY (argv));
 
-    /* The ugly cast is because gnulib gets the const-ness wrong.  */
-    pid = create_pipe_bidi ("m4", m4, (char **)(void*)argv, false, true,
-                            true, filter_fd);
-  }
+  out = xfopen (tmp_file_1, "w");
+    if (! out)
+    error (EXIT_FAILURE, get_errno (),
+       "xfopen");
+if (trace_flag & trace_muscles)
+    muscles_output (stderr);
+    muscles_output (out);
+  xfclose (out);
+
+  if ((pid = fork()) == 0) {   /* Child runs user job */
+           if (execve(argv[0], (char **const)argv, environ) < 0) {
+           printf("%s: Command not found.\n", argv[0]);
+        exit(0);
+        }
+    }
+  int status;
+        if (waitpid(pid, &status, 0) < 0)
+           error(0,0,"waitpid error");
 
   free (m4sugar);
   free (m4bison);
   free (skel);
 
-  if (trace_flag & trace_muscles)
-    muscles_output (stderr);
-  {
-    FILE *out = xfdopen (filter_fd[1], "w");
-    muscles_output (out);
-    xfclose (out);
-  }
 
   /* Read and process m4's output.  */
   timevar_push (TV_M4);
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
-  timevar_pop (TV_M4);
+
+  in = xfopen (tmp_file_2, "r");
+  if (! in)
+    error (EXIT_FAILURE, get_errno (),
+       "xfopen");
+  scan_skel (in);
+  xfclose (in);
+  remove (tmp_file_1);
+  remove (tmp_file_2);
+  timevar_pop (TV_M4); 
 }
 
 static void
