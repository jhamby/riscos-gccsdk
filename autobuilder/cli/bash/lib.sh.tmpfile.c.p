--- lib/sh/tmpfile.c.orig	2020-06-05 10:46:55.000000000 -0700
+++ lib/sh/tmpfile.c	2023-07-30 16:14:39.309000262 -0700
@@ -270,7 +270,7 @@
   char *filename, *tdir, *lroot, *dirname;
   int fd, tdlen;
   
-#ifdef USE_MKDTEMP
+#if defined(USE_MKDTEMP) && defined(HAVE_MKDTEMP)
   filename = (char *)xmalloc (PATH_MAX + 1);
   tdir = get_tmpdir (flags);
   tdlen = strlen (tdir);
