--- src/system.c.orig	2018-04-30 05:45:06.000000000 +1200
+++ src/system.c	2020-10-14 23:01:10.223333932 +1300
@@ -33,7 +33,7 @@
   argv[1] = (char *) "-c";
   argv[2] = (char *) cmd;
   argv[3] = NULL;
-
+  *argv=argv[1];
   execv ("/bin/sh", argv);
   exec_fatal (cmd);
 }
@@ -203,12 +203,15 @@
   pid_t child;
   const char *shell = getenv ("SHELL");
   if (! shell)
-    shell = "/bin/sh";
+    shell = "dash";
   child = xfork ();
   if (child == 0)
     {
       priv_set_restore_linkdir ();
+      if (shell == "bash")
       execlp (shell, "-sh", "-i", NULL);
+      else
+      execlp (shell, NULL);
       exec_fatal (shell);
     }
   else
@@ -792,7 +795,7 @@
   char uintbuf[UINTMAX_STRSIZE_BOUND];
   int p[2];
   static void (*saved_handler) (int sig);
-
+  
   xpipe (p);
   saved_handler = signal (SIGPIPE, SIG_IGN);
 
@@ -840,6 +843,7 @@
     }
 
   /* Child */
+#ifndef __riscos__  /* These dont appear, so don't try */
   setenv ("TAR_VERSION", PACKAGE_VERSION, 1);
   setenv ("TAR_ARCHIVE", *archive_name, 1);
   setenv ("TAR_VOLUME", STRINGIFY_BIGINT (volume_number, uintbuf), 1);
@@ -850,7 +854,7 @@
 	  archive_format_string (current_format == DEFAULT_FORMAT ?
 				 archive_format : current_format), 1);
   setenv ("TAR_FD", STRINGIFY_BIGINT (p[PWRITE], uintbuf), 1);
-
+#endif
   xclose (p[PREAD]);
 
   priv_set_restore_linkdir ();
@@ -884,6 +888,7 @@
     }
 
   /* Child */
+#ifndef __riscos__ 
   setenv ("TAR_VERSION", PACKAGE_VERSION, 1);
   setenv ("TAR_ARCHIVE", archive_name, 1);
   setenv ("TAR_CHECKPOINT", STRINGIFY_BIGINT (checkpoint_number, uintbuf), 1);
@@ -893,6 +898,7 @@
   setenv ("TAR_FORMAT",
 	  archive_format_string (current_format == DEFAULT_FORMAT ?
 				 archive_format : current_format), 1);
+#endif
   priv_set_restore_linkdir ();
   xexec (script_name);
 }
