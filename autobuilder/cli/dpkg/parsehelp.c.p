--- lib/dpkg/parsehelp.c.orig	2010-06-10 23:25:49.831177122 +0100
+++ lib/dpkg/parsehelp.c	2010-06-10 23:27:48.091233694 +0100
@@ -220,16 +220,16 @@
   if (!*string) return _("version string is empty");
 
   /* trim leading and trailing space */
-  while (*string && isblank(*string))
+  while (*string && isspace(*string))
     string++;
   /* string now points to the first non-whitespace char */
   end = string;
   /* find either the end of the string, or a whitespace char */
-  while (*end && !isblank(*end))
+  while (*end && !isspace(*end))
     end++;
   /* check for extra chars after trailing space */
   ptr = end;
-  while (*ptr && isblank(*ptr))
+  while (*ptr && isspace(*ptr))
     ptr++;
   if (*ptr) return _("version string has embedded spaces");
 
