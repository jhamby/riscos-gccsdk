--- libcpp/errors.c.orig	2008-12-08 03:29:05.000000000 +0100
+++ libcpp/errors.c	2008-12-08 03:33:39.000000000 +0100
@@ -30,6 +30,9 @@ Foundation, 51 Franklin Street, Fifth Fl
 
 static void print_location (cpp_reader *, source_location, unsigned int);
 
+const char *throwback_file = "";
+int throwback_line = 0;
+
 /* Print the logical file location (LINE, COL) in preparation for a
    diagnostic.  Outputs the #include chain if it has changed.  A line
    of zero suppresses the include stack, and outputs the program name
@@ -62,6 +65,9 @@ print_location (cpp_reader *pfile, sourc
       else
 	fprintf (stderr, "%s:%u:%u:", map->to_file, lin, col);
 
+      throwback_file = map->to_file;
+      throwback_line = lin;
+
       fputc (' ', stderr);
     }
 }
@@ -157,7 +163,15 @@ cpp_error (cpp_reader * pfile, int level
 	}
 
       if (_cpp_begin_message (pfile, level, src_loc, 0))
-	v_message (msgid, ap);
+        {
+          va_list ap2;
+
+          va_copy (ap2, ap);
+          v_message (msgid, ap);
+	  if (pfile->cb.throwback)
+            pfile->cb.throwback (level, throwback_file, throwback_line, _(msgid), &ap2);
+	  va_end (ap2);
+        }
     }
 
   va_end (ap);
@@ -174,7 +188,15 @@ cpp_error_with_line (cpp_reader *pfile, 
   va_start (ap, msgid);
 
   if (_cpp_begin_message (pfile, level, src_loc, column))
-    v_message (msgid, ap);
+    {
+      va_list ap2;
+
+      va_copy (ap2, ap);
+      v_message (msgid, ap);
+      if (pfile->cb.throwback)
+        pfile->cb.throwback (level, throwback_file, throwback_line, _(msgid), &ap2);
+      va_end (ap2);
+    }
 
   va_end (ap);
 }
