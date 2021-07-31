--- lib/fseterr.c.orig	2015-01-05 05:43:50.000000000 +1300
+++ lib/fseterr.c	2015-03-08 22:44:48.604301678 +1300
@@ -20,6 +20,8 @@
 #include "fseterr.h"
 
 #include <errno.h>
+#include <unistd.h>
+#include <stdlib.h>
 
 #include "stdio-impl.h"
 
@@ -48,7 +50,7 @@
 #elif defined EPLAN9                /* Plan9 */
   if (fp->state != 0 /* CLOSED */)
     fp->state = 5 /* ERR */;
-#elif 0                             /* unknown  */
+#elif defined __riscos                             /* unknown  */
   /* Portable fallback, based on an idea by Rich Felker.
      Wow! 6 system calls for something that is just a bit operation!
      Not activated on any system, because there is no way to repair FP when
