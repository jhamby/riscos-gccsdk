--- binutils/ar.c.orig	2009-02-01 17:59:33.000000000 +0100
+++ binutils/ar.c	2009-02-01 17:59:23.000000000 +0100
@@ -779,10 +779,10 @@ open_inarch (const char *archive_filenam
 static void
 print_contents (bfd *abfd)
 {
-  int ncopied = 0;
+  size_t ncopied = 0;
   char *cbuf = xmalloc (BUFSIZE);
   struct stat buf;
-  long size;
+  size_t size;
   if (bfd_stat_arch_elt (abfd, &buf) != 0)
     /* xgettext:c-format */
     fatal (_("internal stat error on %s"), bfd_get_filename (abfd));
@@ -797,8 +797,8 @@ print_contents (bfd *abfd)
   while (ncopied < size)
     {
 
-      int nread;
-      int tocopy = size - ncopied;
+      size_t nread;
+      size_t tocopy = size - ncopied;
       if (tocopy > BUFSIZE)
 	tocopy = BUFSIZE;
 
@@ -807,7 +807,12 @@ print_contents (bfd *abfd)
 	/* xgettext:c-format */
 	fatal (_("%s is not a valid archive"),
 	       bfd_get_filename (bfd_my_archive (abfd)));
-      fwrite (cbuf, 1, nread, stdout);
+
+      /* fwrite in mingw32 may return int instead of size_t. Cast the
+	 return value to size_t to avoid comparison between signed and
+	 unsigned values.  */
+      if ((size_t) fwrite (cbuf, 1, nread, stdout) != nread)
+	fatal ("stdout: %s", strerror (errno));
       ncopied += tocopy;
     }
   free (cbuf);
@@ -828,9 +833,9 @@ extract_file (bfd *abfd)
 {
   FILE *ostream;
   char *cbuf = xmalloc (BUFSIZE);
-  int nread, tocopy;
-  long ncopied = 0;
-  long size;
+  size_t nread, tocopy;
+  size_t ncopied = 0;
+  size_t size;
   struct stat buf;
 
   if (bfd_stat_arch_elt (abfd, &buf) != 0)
@@ -838,10 +843,6 @@ extract_file (bfd *abfd)
     fatal (_("internal stat error on %s"), bfd_get_filename (abfd));
   size = buf.st_size;
 
-  if (size < 0)
-    /* xgettext:c-format */
-    fatal (_("stat returns negative size for %s"), bfd_get_filename (abfd));
-
   if (verbose)
     printf ("x - %s\n", bfd_get_filename (abfd));
 
@@ -890,7 +891,12 @@ extract_file (bfd *abfd)
 
 	    output_file = ostream;
 	  }
-	fwrite (cbuf, 1, nread, ostream);
+
+	/* fwrite in mingw32 may return int instead of size_t. Cast
+	   the return value to size_t to avoid comparison between
+	   signed and unsigned values.  */
+	if ((size_t) fwrite (cbuf, 1, nread, ostream) != nread)
+	  fatal ("%s: %s", output_filename, strerror (errno));
 	ncopied += tocopy;
       }
 
@@ -923,6 +929,9 @@ write_archive (bfd *iarch)
   old_name = xmalloc (strlen (bfd_get_filename (iarch)) + 1);
   strcpy (old_name, bfd_get_filename (iarch));
   new_name = make_tempname (old_name);
+ 
+  if (new_name == NULL)
+    bfd_fatal ("could not create temporary file whilst writing archive");
 
   output_filename = new_name;
 
