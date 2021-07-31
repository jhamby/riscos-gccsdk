--- findcmd.c.orig	2005-04-03 22:42:48.130000000 +0100
+++ findcmd.c	2005-04-03 22:42:23.420000000 +0100
@@ -520,9 +520,14 @@
       return (full_path);
     }
 
+  /* For RISC OS $PATH is not set up correctly yet, and if it was it wouldn't
+     take alias$foo into account, so pretend the file was found. */
+  return (savestring (name));
+
   if (path_list == 0 || *path_list == '\0')
     return (savestring (name));		/* XXX */
 
+
   file_to_lose_on = (char *)NULL;
   name_len = strlen (name);
   stat (".", &dotinfo);
