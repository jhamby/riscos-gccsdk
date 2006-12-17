--- ../orig.binutils/ld/ldlang.c	2006-06-12 06:05:04.000000000 -0700
+++ ld/ldlang.c	2006-12-17 08:52:44.000000000 -0800
@@ -5234,6 +5234,17 @@
   /* Make -o on command line override OUTPUT in script.  */
   if (!had_output_filename || !from_script)
     {
+      /* Ensure output filename has ,e1f for RISC OS executables when cross-compiling */
+#ifndef __riscos__
+      int len = strlen (output_filename);
+
+      if (len > 4 && strcmp (output_filename + len - 4, ",e1f") != 0) {
+         char *new_name = xmalloc (len + 5);
+
+         snprintf(new_name, len + 5, "%s,e1f", name);
+         name = new_name;
+      }
+#endif
       output_filename = name;
       had_output_filename = TRUE;
     }
