--- util/utils.h.orig	2009-08-27 11:34:32.000000000 -0700
+++ util/utils.h	2009-08-27 11:34:39.000000000 -0700
@@ -29,6 +29,7 @@
 #define NEDIT_UTILS_H_INCLUDED
 
 #include <sys/utsname.h>
+#include <stddef.h>
 
 #ifdef VMS
 #include "vmsparam.h"
