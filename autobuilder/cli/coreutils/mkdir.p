--- src/mkdir.c.orig	2015-07-13 13:13:07.969297000 +0100
+++ src/mkdir.c	2015-07-14 13:52:11.452605051 +0100
@@ -202,6 +202,12 @@
   int optc;
   char const *scontext = NULL;
   struct mkdir_options options;
+#ifdef __riscos__
+  /* RISC OS port was changing directory when it created parents
+     so save it and change it back to be safe. */     
+  char current_dir[PATH_MAX+1];
+  current_dir[0] = 0;
+#endif   
 
   options.make_ancestor_function = NULL;
   options.mode = S_IRWXUGO;
@@ -223,6 +229,9 @@
         {
         case 'p':
           options.make_ancestor_function = make_ancestor;
+#ifdef __riscos__
+          if (!getcwd(current_dir,PATH_MAX+1)) current_dir[0] = 0;
+#endif   
           break;
         case 'm':
           specified_mode = optarg;
@@ -301,6 +310,15 @@
         options.mode = S_IRWXUGO;
     }
 
+#ifdef __riscos__
+    if (current_dir[0])
+    {
+       int exit_val = savewd_process_files (argc - optind, argv + optind,
+                             process_dir, &options);
+       int dont_care = chdir(current_dir);                                    
+       exit (exit_val);
+    } else
+#endif                              
   exit (savewd_process_files (argc - optind, argv + optind,
                               process_dir, &options));
 }
