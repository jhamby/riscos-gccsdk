--- src/common/strcasecmp.c.old	2003-01-31 20:50:12.000000000 +0000
+++ src/common/strcasecmp.c	2003-01-31 20:51:06.000000000 +0000
@@ -26,6 +26,7 @@
 #include <stdio.h>
 #include <ctype.h>
 
+#ifndef __riscos__
 
 /* prototypes to shut up strict compiler. */
 int strcasecmp(const char *str1, const char *str2);
@@ -72,3 +73,5 @@
 
     return (c1 - c2);
 }
+
+#endif
