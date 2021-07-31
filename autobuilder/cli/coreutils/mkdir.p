--- src/mkdir.c.orig	2016-11-28 16:17:17.000000000 +0000
+++ src/mkdir.c	2019-12-01 17:44:03.370282355 +0000
@@ -192,6 +192,12 @@
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
@@ -213,6 +219,9 @@
         {
         case 'p':
           options.make_ancestor_function = make_ancestor;
+#ifdef __riscos__
+          if (!getcwd(current_dir,PATH_MAX+1)) current_dir[0] = 0;
+#endif   
           break;
         case 'm':
           specified_mode = optarg;
@@ -291,6 +300,15 @@
         options.mode = S_IRWXUGO;
     }
 
+#ifdef __riscos__
+    if (current_dir[0])
+    {
+       int exit_val = savewd_process_files (argc - optind, argv + optind,
+                             process_dir, &options);
+       int dont_care = chdir(current_dir);                                    
+       exit (exit_val);
+    }
+#endif 
   return savewd_process_files (argc - optind, argv + optind,
                                process_dir, &options);
 }
