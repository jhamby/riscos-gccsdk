--- binutils/readelf.c.orig	2008-12-10 00:07:37.000000000 +0100
+++ binutils/readelf.c	2008-12-10 00:09:12.000000000 +0100
@@ -47,6 +47,25 @@
 #include <stdio.h>
 #include <time.h>
 
+/* for PATH_MAX */
+#ifdef HAVE_LIMITS_H
+#include <limits.h>
+#endif
+
+#ifndef PATH_MAX
+/* for MAXPATHLEN */
+# ifdef HAVE_SYS_PARAM_H
+#  include <sys/param.h>
+# endif
+# ifndef PATH_MAX
+#  ifdef MAXPATHLEN
+#   define PATH_MAX MAXPATHLEN
+#  else
+#   define PATH_MAX 1024
+#  endif
+# endif
+#endif
+
 #if __GNUC__ >= 2
 /* Define BFD64 here, even if our default architecture is 32 bit ELF
    as this will allow us to read in and parse 64bit and 32bit ELF files.
@@ -3435,8 +3454,15 @@ process_program_headers (FILE *file)
 	    error (_("Unable to find program interpreter name\n"));
 	  else
 	    {
+	      char fmt [32];
+	      int ret = snprintf (fmt, sizeof (fmt), "%%%ds", PATH_MAX);
+
+	      if (ret >= (int) sizeof (fmt) || ret < 0)
+		error (_("Internal error: failed to create format string to display program interpreter\n"));
+
 	      program_interpreter[0] = 0;
-	      fscanf (file, "%63s", program_interpreter);
+	      if (fscanf (file, fmt, program_interpreter) <= 0)
+		error (_("Unable to read program interpreter name\n"));
 
 	      if (do_segments)
 		printf (_("\n      [Requesting program interpreter: %s]"),
