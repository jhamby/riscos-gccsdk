--- libavformat/avio.c.orig	2017-11-05 14:58:09.150165307 +0000
+++ libavformat/avio.c	2017-11-05 15:07:11.242766616 +0000
@@ -257,7 +257,8 @@
 
     if (filename[proto_len] != ':' &&
         (strncmp(filename, "subfile,", 8) || !strchr(filename + proto_len + 1, ':')) ||
-        is_dos_path(filename))
+        is_dos_path(filename) || ((filename[proto_len] == ':') &&
+        (filename[proto_len+1] == ':')))
         strcpy(proto_str, "file");
     else
         av_strlcpy(proto_str, filename,
