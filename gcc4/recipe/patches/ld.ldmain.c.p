--- ld/ldmain.c.orig	2006-10-09 02:59:52.000000000 +0200
+++ ld/ldmain.c	2006-10-09 03:00:03.000000000 +0200
@@ -501,8 +501,12 @@
 	  int len = strlen (output_filename);
 
 	  if (len < 4
+#ifdef __riscos__
+		  && strcmp (output_filename + len - 4, ",e1f") != 0))
+#else
 	      || (strcasecmp (output_filename + len - 4, ".exe") != 0
 		  && strcasecmp (output_filename + len - 4, ".dll") != 0))
+#endif
 	    {
 	      FILE *src;
 	      FILE *dst;
@@ -512,7 +516,11 @@
 	      char *dst_name = xmalloc (len + 5);
 
 	      strcpy (dst_name, output_filename);
+#ifdef __riscos__
+	      strcat (dst_name, ",e1f");
+#else
 	      strcat (dst_name, ".exe");
+#endif
 	      src = fopen (output_filename, FOPEN_RB);
 	      dst = fopen (dst_name, FOPEN_WB);
 
