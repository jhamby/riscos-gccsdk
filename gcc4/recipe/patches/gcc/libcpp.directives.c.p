Index: libcpp/directives.c
===================================================================
--- libcpp/directives.c	(revision 170537)
+++ libcpp/directives.c	(working copy)
@@ -704,9 +704,13 @@
   if ((header->type == CPP_STRING && header->val.str.text[0] != 'R')
       || header->type == CPP_HEADER_NAME)
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
