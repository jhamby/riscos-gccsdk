--- lib/group-member.c.orig	2018-01-21 23:00:57.000000000 +1300
+++ lib/group-member.c	2018-03-22 00:13:24.436647986 +1300
@@ -19,11 +19,17 @@
 #include <config.h>
 
 /* Specification.  */
+#ifndef __riscos__
 #include <unistd.h>
+#include <stdlib.h>
+#endif
+#ifdef __riscos__
+#include <sys/select.h>
+#endif
 
 #include <stdio.h>
 #include <sys/types.h>
-#include <stdlib.h>
+
 
 #include "xalloc-oversized.h"
 
