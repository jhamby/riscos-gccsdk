--- !DLSources/Libraries/MsgTrans/LoadFile.c.orig	2023-08-11 16:16:35.642585231 -0700
+++ !DLSources/Libraries/MsgTrans/LoadFile.c	2023-08-11 16:32:56.945421078 -0700
@@ -58,7 +58,7 @@
     if (!File_Exists(namebuff))
     {
       /* Still can't find file */
-      snprintf(error.errmess, sizeof(error.errmess), "Messages file not found. Tried '%s' and '%s'", filename, namebuff);
+      snprintf(error.errmess, sizeof(error.errmess), "Messages file not found. Tried '%.92s' and '%.92s'", filename, namebuff);
       error.errnum = 1;
       return &error;
     }
