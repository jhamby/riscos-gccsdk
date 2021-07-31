--- glib/gstrfuncs.c.orig	2014-02-22 15:29:07.000000000 +0000
+++ glib/gstrfuncs.c	2014-05-08 17:08:34.140137594 +0100
@@ -28,6 +28,7 @@
 
 #include "config.h"
 
+#include <features.h>
 #include <stdarg.h>
 #include <stdio.h>
 #include <stdlib.h>
