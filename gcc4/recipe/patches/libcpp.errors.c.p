--- libcpp/errors.c.orig	2007-12-22 14:03:05.000000000 +0100
+++ libcpp/errors.c	2007-12-22 14:02:27.000000000 +0100
@@ -30,6 +30,9 @@
 
 static void print_location (cpp_reader *, source_location, unsigned int);
 
+const char *throwback_file = "";
+int throwback_line = 0;
+
 /* Print the logical file location (LINE, COL) in preparation for a
    diagnostic.  Outputs the #include chain if it has changed.  A line
    of zero suppresses the include stack, and outputs the program name
@@ -62,6 +65,9 @@
       else
 	fprintf (stderr, "%s:%u:%u:", map->to_file, lin, col);
 
+      throwback_file = map->to_file;
+      throwback_line = lin;
+
       fputc (' ', stderr);
     }
 }
@@ -157,7 +163,15 @@
 	}
 
       if (_cpp_begin_message (pfile, level, src_loc, 0))
-	v_message (msgid, ap);
+        {
+          va_list ap2;
+
+          va_copy (ap2, ap);
+          v_message (msgid, ap);
+	  if (pfile->cb.throwback)
+            (pfile->cb.throwback) (level, throwback_file, throwback_line, _(msgid), ap2);
+	  va_end (ap2);
+        }
     }
 
   va_end (ap);
@@ -174,7 +188,15 @@
   va_start (ap, msgid);
 
   if (_cpp_begin_message (pfile, level, src_loc, column))
-    v_message (msgid, ap);
+    {
+      va_list ap2;
+
+      va_copy (ap2, ap);
+      v_message (msgid, ap);
+      if (pfile->cb.throwback)
+        (*pfile->cb.throwback) (level, throwback_file, throwback_line, _(msgid), ap2);
+      va_end (ap2);
+    }
 
   va_end (ap);
 }
