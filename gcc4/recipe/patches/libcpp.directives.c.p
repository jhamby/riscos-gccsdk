--- libcpp/directives.c.orig	2008-12-13 20:11:03.000000000 +0100
+++ libcpp/directives.c	2008-12-13 20:39:07.000000000 +0100
@@ -675,9 +675,13 @@ parse_include (cpp_reader *pfile, int *p
   header = get_token_no_padding (pfile);
   if (header->type == CPP_STRING || header->type == CPP_HEADER_NAME)
     {
+      extern char *riscos_to_unix (char *);
+
       fname = XNEWVEC (char, header->val.str.len - 1);
       memcpy (fname, header->val.str.text + 1, header->val.str.len - 2);
       fname[header->val.str.len - 2] = '\0';
+      /* Convert the #include filename to a Unix version.  */
+      fname = riscos_to_unix (fname);
       *pangle_brackets = header->type == CPP_HEADER_NAME;
     }
   else if (header->type == CPP_LESS)
@@ -985,6 +989,18 @@ _cpp_do_file_change (cpp_reader *pfile, 
     pfile->cb.file_change (pfile, map);
 }
 
+static void ATTRIBUTE_PRINTF_5
+do_diagnostic_throwback (cpp_reader *pfile, int code, const char *tb_file,
+			 int tb_line, const char *s, ...)
+{
+  va_list va;
+
+  va_start (va, s);
+  if (pfile->cb.throwback)
+    pfile->cb.throwback (code, tb_file, tb_line, s, &va);
+  va_end (va);
+}
+
 /* Report a warning or error detected by the program we are
    processing.  Use the directive's tokens in the error message.  */
 static void
@@ -992,11 +1008,16 @@ do_diagnostic (cpp_reader *pfile, int co
 {
   if (_cpp_begin_message (pfile, code, pfile->cur_token[-1].src_loc, 0))
     {
+      extern const char *throwback_file;
+      extern int throwback_line;
+
       if (print_dir)
 	fprintf (stderr, "#%s ", pfile->directive->name);
       pfile->state.prevent_expansion++;
       cpp_output_line (pfile, stderr);
       pfile->state.prevent_expansion--;
+      do_diagnostic_throwback (pfile, code, throwback_file, throwback_line,
+			       "%s", pfile->directive->name);
     }
 }
 
