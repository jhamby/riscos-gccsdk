--- libcpp/directives.c.orig	2007-12-21 00:07:45.000000000 +0100
+++ libcpp/directives.c	2007-12-19 02:23:06.000000000 +0100
@@ -675,9 +679,13 @@
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
@@ -997,6 +1005,13 @@
       pfile->state.prevent_expansion++;
       cpp_output_line (pfile, stderr);
       pfile->state.prevent_expansion--;
+      if (pfile->cb.throwback)
+        {
+          extern const char *throwback_file;
+          extern int throwback_line;
+
+          (*pfile->cb.throwback) (code, throwback_file, throwback_line, "%s", pfile->directive->name);
+        }
     }
 }
 
