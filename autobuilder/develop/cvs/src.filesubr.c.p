--- src/filesubr.c.orig	2011-02-07 01:06:48.683002936 +0100
+++ src/filesubr.c	2011-02-07 01:07:17.313001615 +0100
@@ -22,6 +22,10 @@
 #include "save-cwd.h"
 #include "xsize.h"
 
+#ifdef __riscos__
+#  include <unixlib/local.h>
+#endif
+
 static int deep_remove_dir (const char *path);
 
 /*
@@ -40,6 +44,7 @@
     if (noexec)
 	return;
 
+#ifdef HAVE_READLINK
     /* If the file to be copied is a link or a device, then just create
        the new link or device appropriately. */
     if ((rsize = islink (from)) > 0)
@@ -49,6 +54,7 @@
 	free (source);
 	return;
     }
+#endif
 
     if (isdevice (from))
     {
@@ -582,6 +588,7 @@
 
     /* If FILE1 and FILE2 are symlinks, they are equal if they point to
        the same thing. */
+#ifdef HAVE_READLINK
 #ifdef S_ISLNK
     if (S_ISLNK (sb1.st_mode) && S_ISLNK (sb2.st_mode))
     {
@@ -594,6 +601,7 @@
 	return result;
     }
 #endif
+#endif
 
     /* If FILE1 and FILE2 are devices, they are equal if their device
        numbers match. */
@@ -761,6 +769,7 @@
 
 
 
+#ifndef __riscos__
 /* Return a pointer into PATH's last component.  */
 const char *
 last_component (const char *path)
@@ -772,6 +781,24 @@
     else
         return path;
 }
+#else
+/* Return a pointer into PATH's last component :
+ *  path                        return
+ *  /RAM::RamDisc0/$/test/sub   sub
+ *  /RAM::RamDisc0/$/test       test
+ *  /RAM::RamDisc0/$            /RAM::RamDisc0/$  [!!!]
+ */
+const char *
+last_component (const char *path)
+{
+    const char *last = strrchr (path, '/');
+ 
+    if (last && (last != path) && !(last[1] == '$' && last[2] == '\0'))
+        return last + 1;
+    else
+        return path;
+}
+#endif
 
 
 
@@ -800,6 +827,7 @@
    .cvsrc and such, in the cases where it works.  */
 char *
 get_homedir (void)
+#ifndef __riscos__
 {
     static char *home = NULL;
     char *env;
@@ -818,6 +846,56 @@
 
     return home;
 }
+#else
+{
+    static char *buf = NULL;
+    char *home, *result;
+    int buflen;
+    int free_home;
+
+    /* Fetch cached result, otherwise big memory leak */
+    if (buf) return(buf);
+
+    /* First look for <CVS$HomeDir> */
+    home = getenv ("CVS$HomeDir");
+
+    /* if not found, look for <Choices$Write> */
+    if (home == NULL)
+    {
+      buf = getenv ("Choices$Write");
+      if (buf == NULL) return (NULL);
+
+      buflen = strlen (buf);
+      home = xmalloc(buflen + sizeof(".CVS"));
+      free_home = 1;
+      strcpy (home, buf);
+      strcpy (&home[buflen], ".CVS");
+    }
+    else
+      free_home = 0;
+
+    buflen = 2*strlen (home) + 1; /* Should be sufficent */
+    buf = xmalloc (buflen);
+    /* More or less internal UnixLib call - might break in later versions */
+    result = __unixify (home, 0, buf, buflen, __RISCOSIFY_FILETYPE_NOTSPECIFIED);
+
+    if (free_home)
+    {
+      free (home);
+      home = NULL;
+    }
+
+    if (result == NULL)
+    {
+        free (buf);
+        buf = NULL;
+    }
+    else
+      mkdir_if_needed (buf); /* Ignore return value */
+
+    return buf;
+}
+#endif
 
 /* Compose a path to a file in the home directory.  This is necessary because
  * of different behavior on UNIX and VMS.  See the notes in vms/filesubr.c.
