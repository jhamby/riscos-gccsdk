--- binutils/objcopy.c.orig	2006-02-28 17:09:01.000000000 +0100
+++ binutils/objcopy.c	2009-02-01 17:27:35.000000000 +0100
@@ -1686,13 +1686,6 @@ copy_object (bfd *ibfd, bfd *obfd)
   return TRUE;
 }
 
-#undef MKDIR
-#if defined (_WIN32) && !defined (__CYGWIN32__)
-#define MKDIR(DIR, MODE) mkdir (DIR)
-#else
-#define MKDIR(DIR, MODE) mkdir (DIR, MODE)
-#endif
-
 /* Read each archive element in turn from IBFD, copy the
    contents to temp file, and keep the temp file handle.  */
 
@@ -1707,12 +1700,13 @@ copy_archive (bfd *ibfd, bfd *obfd, cons
     } *list, *l;
   bfd **ptr = &obfd->archive_head;
   bfd *this_element;
-  char *dir = make_tempname (bfd_get_filename (obfd));
+  char * dir;
 
   /* Make a temp directory to hold the contents.  */
-  if (MKDIR (dir, 0700) != 0)
-    fatal (_("cannot mkdir %s for archive copying (error: %s)"),
-	   dir, strerror (errno));
+  dir = make_tempdir (bfd_get_filename (obfd));
+  if (dir == NULL)
+      fatal (_("cannot create tempdir for archive copying (error: %s)"),
+	   strerror (errno));
 
   obfd->has_armap = ibfd->has_armap;
 
@@ -1739,10 +1733,10 @@ copy_archive (bfd *ibfd, bfd *obfd, cons
       /* If the file already exists, make another temp dir.  */
       if (stat (output_name, &buf) >= 0)
 	{
-	  output_name = make_tempname (output_name);
-	  if (MKDIR (output_name, 0700) != 0)
-	    fatal (_("cannot mkdir %s for archive copying (error: %s)"),
-		   output_name, strerror (errno));
+	  output_name = make_tempdir (output_name);
+	  if (output_name == NULL)
+	    fatal (_("cannot create tempdir for archive copying (error: %s)"),
+		   strerror (errno));
 
 	  l = xmalloc (sizeof (struct name_list));
 	  l->name = output_name;
@@ -2597,8 +2591,16 @@ strip_main (int argc, char *argv[])
 	tmpname = output_file;
       else
 	tmpname = make_tempname (argv[i]);
-      status = 0;
+ 
+      if (tmpname == NULL)
+	{
+	  non_fatal (_("could not create temporary file to hold stripped copy of '%s'"),
+		     argv[i]);
+	  status = 1;
+	  continue;
+	}
 
+      status = 0;
       copy_file (argv[i], tmpname, input_target, output_target);
       if (status == 0)
 	{
@@ -3190,6 +3192,10 @@ copy_main (int argc, char *argv[])
   if (output_filename == NULL || strcmp (input_filename, output_filename) == 0)
     {
       char *tmpname = make_tempname (input_filename);
+ 
+      if (tmpname == NULL)
+	fatal (_("warning: could not create temporary file whilst copying '%s', (error: %s)"),
+	       input_filename, strerror (errno));
 
       copy_file (input_filename, tmpname, input_target, output_target);
       if (status == 0)
