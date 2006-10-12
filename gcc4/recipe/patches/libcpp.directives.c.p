--- libcpp/directives.c.orig	2006-10-12 01:31:39.000000000 +0200
+++ libcpp/directives.c	2006-10-12 00:47:15.000000000 +0200
@@ -675,9 +675,19 @@
   header = get_token_no_padding (pfile);
   if (header->type == CPP_STRING || header->type == CPP_HEADER_NAME)
     {
+      char unixed[256]; /* Temp buffer to translate Unix filenames to RISC OS */
+      int len;
+      extern char *riscos_to_unix (char *, char *);
+
       fname = XNEWVEC (char, header->val.str.len - 1);
       memcpy (fname, header->val.str.text + 1, header->val.str.len - 2);
       fname[header->val.str.len - 2] = '\0';
+      /* Convert the #include filename to a Unix version.  */
+      riscos_to_unix (fname, unixed);
+      free (fname);
+      len = strlen (unixed) + 1;
+      fname = xmalloc (len);
+      memcpy (fname, unixed, len);
       *pangle_brackets = header->type == CPP_HEADER_NAME;
     }
   else if (header->type == CPP_LESS)
