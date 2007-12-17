--- libcpp/directives.c.orig	2007-12-17 00:15:32.000000000 +0100
+++ libcpp/directives.c	2007-12-16 23:53:53.000000000 +0100
@@ -675,9 +675,13 @@
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
