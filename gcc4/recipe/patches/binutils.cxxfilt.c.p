--- binutils/cxxfilt.c.orig	2008-12-10 00:11:49.000000000 +0100
+++ binutils/cxxfilt.c	2008-12-10 00:11:39.000000000 +0100
@@ -63,12 +63,12 @@ demangle_it (char *mangled_name)
   result = cplus_demangle (mangled_name + skip_first, flags);
 
   if (result == NULL)
-    printf (mangled_name);
+    printf ("%s", mangled_name);
   else
     {
       if (mangled_name[0] == '.')
 	putchar ('.');
-      printf (result);
+      printf ("%s", result);
       free (result);
     }
 }
