--- tools/compat.c.orig	2011-12-18 10:35:39.000000000 +0000
+++ tools/compat.c	2011-12-18 10:36:01.000000000 +0000
@@ -36,6 +36,7 @@
 
 #include "compat.h"
 
+#ifndef __riscos__
 #ifndef HAVE_ERR_H
 
 #include <stdlib.h>
@@ -226,3 +227,4 @@
 }
 
 #endif /* HAVE_ERR_H */
+#endif
