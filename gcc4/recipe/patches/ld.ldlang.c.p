--- ld/ldlang.c.orig	2006-12-23 16:22:48.000000000 -0800
+++ ld/ldlang.c	2006-12-23 15:54:33.000000000 -0800
@@ -5234,6 +5234,17 @@
   /* Make -o on command line override OUTPUT in script.  */
   if (!had_output_filename || !from_script)
     {
+      /* Ensure output filename has ,e1f for RISC OS executables when cross-compiling */
+#ifndef __riscos__
+      int len = strlen (name);
+
+      if (!getenv("GCCSDK_NOE1F") && strchr(name, '.') == NULL && (len < 4 || (strcmp (name + len - 4, ",e1f") != 0))) {
+         char *new_name = xmalloc (len + 5);
+
+         snprintf(new_name, len + 5, "%s,e1f", name);
+         name = new_name;
+      }
+#endif
       output_filename = name;
       had_output_filename = TRUE;
     }
