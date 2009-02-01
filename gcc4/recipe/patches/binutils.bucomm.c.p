--- binutils/bucomm.c.orig	2009-02-01 17:12:45.000000000 +0100
+++ binutils/bucomm.c	2009-02-01 17:12:36.000000000 +0100
@@ -1,5 +1,6 @@
 /* bucomm.c -- Bin Utils COMmon code.
-   Copyright 1991, 1992, 1993, 1994, 1995, 1997, 1998, 2000, 2001, 2002, 2003
+   Copyright 1991, 1992, 1993, 1994, 1995, 1997, 1998, 2000, 2001, 2002,
+   2003, 2006
    Free Software Foundation, Inc.
 
    This file is part of GNU Binutils.
@@ -386,55 +387,103 @@ print_arelt_descr (FILE *file, bfd *abfd
   fprintf (file, "%s\n", bfd_get_filename (abfd));
 }
 
-/* Return the name of a temporary file in the same directory as FILENAME.  */
+/* Return a path for a new temporary file in the same directory
+   as file PATH.  */
 
-char *
-make_tempname (char *filename)
+static char *
+template_in_dir (const char *path)
 {
-  static char template[] = "stXXXXXX";
+#define template "stXXXXXX"
+  const char *slash = strrchr (path, '/');
   char *tmpname;
-  char *slash = strrchr (filename, '/');
+  size_t len;
 
 #ifdef HAVE_DOS_BASED_FILE_SYSTEM
   {
     /* We could have foo/bar\\baz, or foo\\bar, or d:bar.  */
-    char *bslash = strrchr (filename, '\\');
+    char *bslash = strrchr (path, '\\');
+
     if (slash == NULL || (bslash != NULL && bslash > slash))
       slash = bslash;
-    if (slash == NULL && filename[0] != '\0' && filename[1] == ':')
-      slash = filename + 1;
+    if (slash == NULL && path[0] != '\0' && path[1] == ':')
+      slash = path + 1;
   }
 #endif
 
   if (slash != (char *) NULL)
     {
-      char c;
+      len = slash - path;
+      tmpname = xmalloc (len + sizeof (template) + 2);
+      memcpy (tmpname, path, len);
 
-      c = *slash;
-      *slash = 0;
-      tmpname = xmalloc (strlen (filename) + sizeof (template) + 2);
-      strcpy (tmpname, filename);
 #ifdef HAVE_DOS_BASED_FILE_SYSTEM
       /* If tmpname is "X:", appending a slash will make it a root
 	 directory on drive X, which is NOT the same as the current
 	 directory on drive X.  */
-      if (tmpname[1] == ':' && tmpname[2] == '\0')
-	strcat (tmpname, ".");
+      if (len == 2 && tmpname[1] == ':')
+	tmpname[len++] = '.';
 #endif
-      strcat (tmpname, "/");
-      strcat (tmpname, template);
-      mktemp (tmpname);
-      *slash = c;
+      tmpname[len++] = '/';
     }
   else
     {
       tmpname = xmalloc (sizeof (template));
-      strcpy (tmpname, template);
-      mktemp (tmpname);
+      len = 0;
     }
+
+  memcpy (tmpname + len, template, sizeof (template));
+  return tmpname;
+#undef template
+}
+
+/* Return the name of a created temporary file in the same directory
+   as FILENAME.  */
+
+char *
+make_tempname (char *filename)
+{
+  char *tmpname = template_in_dir (filename);
+  int fd;
+
+#ifdef HAVE_MKSTEMP
+  fd = mkstemp (tmpname);
+#else
+  tmpname = mktemp (tmpname);
+  if (tmpname == NULL)
+    return NULL;
+  fd = open (tmpname, O_RDWR | O_CREAT | O_EXCL, 0600);
+#endif
+  if (fd == -1)
+    return NULL;
+  close (fd);
   return tmpname;
 }
 
+/* Return the name of a created temporary directory inside the
+   directory containing FILENAME.  */
+
+char *
+make_tempdir (char *filename)
+{
+  char *tmpname = template_in_dir (filename);
+
+#ifdef HAVE_MKDTEMP
+  return mkdtemp (tmpname);
+#else
+  tmpname = mktemp (tmpname);
+  if (tmpname == NULL)
+    return NULL;
+#if defined (_WIN32) && !defined (__CYGWIN32__)
+  if (mkdir (tmpname) != 0)
+    return NULL;
+#else
+  if (mkdir (tmpname, 0700) != 0)
+    return NULL;
+#endif
+  return tmpname;
+#endif
+}
+
 /* Parse a string into a VMA, with a fatal error if it can't be
    parsed.  */
 
