--- libavformat/avio.c.orig	2011-02-05 14:07:08.000000000 +0000
+++ libavformat/avio.c	2011-02-05 14:07:54.000000000 +0000
@@ -178,7 +178,8 @@
     char proto_str[128];
     size_t proto_len = strspn(filename, URL_SCHEME_CHARS);

-    if (filename[proto_len] != ':' || is_dos_path(filename))
+    if (filename[proto_len] != ':' || is_dos_path(filename) || ((filename[proto_len] == ':') &&
+        (filename[proto_len+1] == ':')))
         strcpy(proto_str, "file");
     else
         av_strlcpy(proto_str, filename, FFMIN(proto_len+1, sizeof(proto_str)));
