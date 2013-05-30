--- glib/gstrfuncs.c.orig	2013-05-29 14:35:31.641208440 +0100
+++ glib/gstrfuncs.c	2013-05-29 14:53:56.949151411 +0100
@@ -30,6 +30,7 @@

 #include "config.h"

+#include <features.h>
 #include <stdarg.h>
 #include <stdio.h>
 #include <stdlib.h>
