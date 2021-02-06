--- wand/mogrify.c.orig	2013-06-14 11:33:00.610029200 +0100
+++ wand/mogrify.c	2013-06-14 11:35:37.039142546 +0100
@@ -54,6 +54,7 @@
 #include "magick/string-private.h"
 #include "magick/utility-private.h"
 
+#include <unixlib/local.h>
 /*
   Define declarations.
 */
@@ -143,8 +144,11 @@
   for (i=1; i < (ssize_t) (argc-1); i++)
   {
     option=argv[i];
-    if ((strlen(option) == 1) || ((*option != '-') && (*option != '+')))
+    if ((strlen(option) == 1) || ((*option != '-') && (*option != '+'))) {
+      if (strlen(option) != 1)
+        option=__unixify(argv[i], 0, NULL, 0, 0);
       continue;
+    }	
     if (LocaleCompare("bench",option+1) == 0)
       iterations=StringToUnsignedLong(argv[++i]);
     if (LocaleCompare("concurrent",option+1) == 0)
