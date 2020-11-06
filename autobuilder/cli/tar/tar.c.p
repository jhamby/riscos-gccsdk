--- src/tar.c.orig	2020-10-08 18:42:55.657065000 +1300
+++ src/tar.c	2020-10-23 21:53:40.231074951 +1300
@@ -74,6 +74,10 @@
 /* Number of allocated tape drive names.  */
 static size_t allocated_archive_names;
 
+/* Used for auto naming multivolumes in buffer.c */
+const char *arg_tarname;
+/* New 'F' option to turn off multivolume stops */
+bool nostop_option = false;
 
 /* Miscellaneous.  */
 
@@ -1413,6 +1417,7 @@
 					 &allocated_archive_names,
 					 sizeof (archive_name_array[0]));
 
+      arg_tarname = arg;
       archive_name_array[archive_names++] = arg;
       break;
 
@@ -1420,6 +1425,11 @@
       /* Since -F is only useful with -M, make it implied.  Run this
 	 script at the end of each tape.  */
 
+      if (strcmp(arg,"nostop") == 0) {
+      nostop_option = true;
+      info_script_option = NULL;
+         }
+      else   
       info_script_option = arg;
       multi_volume_option = true;
       break;
@@ -1921,6 +1931,7 @@
 
     case RMT_COMMAND_OPTION:
       rmt_command = arg;
+      force_local_option = false;
       break;
 
     case RSH_COMMAND_OPTION:
@@ -2277,6 +2288,9 @@
   tar_sparse_major = 1;
   tar_sparse_minor = 0;
 
+  /* RISC OS paths have colons, tar relates them to remote connections,
+     disable that default here and allow it at the rmt_command option */
+  force_local_option = true;
   savedir_sort_order = SAVEDIR_SORT_NONE;
 
   owner_option = -1; owner_name_option = NULL;
