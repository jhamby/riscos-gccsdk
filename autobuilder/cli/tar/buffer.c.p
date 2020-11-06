--- src/buffer.c.orig	2020-10-08 19:02:55.135556000 +1300
+++ src/buffer.c	2020-10-15 02:21:54.439591192 +1300
@@ -37,7 +37,8 @@
 #define READ_ERROR_MAX 10
 
 /* Variables.  */
-
+extern const char* arg_tarname;
+extern bool nostop_option;
 static tarlong prev_written;    /* bytes written on previous volumes */
 static tarlong bytes_written;   /* bytes written on this volume */
 static void *record_buffer[2];  /* allocated memory */
@@ -1214,6 +1215,16 @@
   size_t size = 0;
   bool stop = false;
 
+  char buf[UINTMAX_STRSIZE_BOUND];
+  char *p = STRINGIFY_BIGINT (volno+1, buf);
+  char *s = xmalloc (strlen (arg_tarname) + strlen (p) + 2);
+     sprintf (s, "%s-%s", arg_tarname, p);
+     *archive_name_cursor = s;
+  if (nostop_option) {
+      if (verbose_option)
+      printf("Next volume arg_tarname-volno = %s\n", s);  
+      stop = true;
+      }
   while (!stop)
     {
       fputc ('\007', stderr);
