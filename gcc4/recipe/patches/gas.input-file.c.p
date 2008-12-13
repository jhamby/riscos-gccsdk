--- gas/input-file.c.orig	2008-12-10 00:14:42.000000000 +0100
+++ gas/input-file.c	2008-12-10 00:14:22.000000000 +0100
@@ -166,8 +166,8 @@ input_file_open (char *filename, /* "" m
       c = getc (f_in);
       if (c == 'N')
 	{
-	  fgets (buf, 80, f_in);
-	  if (!strncmp (buf, "O_APP", 5) && ISSPACE (buf[5]))
+	  if (fgets (buf, sizeof (buf), f_in)
+	      && !strncmp (buf, "O_APP", 5) && ISSPACE (buf[5]))
 	    preprocess = 0;
 	  if (!strchr (buf, '\n'))
 	    ungetc ('#', f_in);	/* It was longer.  */
@@ -176,8 +176,8 @@ input_file_open (char *filename, /* "" m
 	}
       else if (c == 'A')
 	{
-	  fgets (buf, 80, f_in);
-	  if (!strncmp (buf, "PP", 2) && ISSPACE (buf[2]))
+	  if (fgets (buf, sizeof (buf), f_in)
+	      && !strncmp (buf, "PP", 2) && ISSPACE (buf[2]))
 	    preprocess = 1;
 	  if (!strchr (buf, '\n'))
 	    ungetc ('#', f_in);
