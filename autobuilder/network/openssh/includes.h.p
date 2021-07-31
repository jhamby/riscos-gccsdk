--- includes.h~	2010-10-24 00:47:30.000000000 +0100
+++ includes.h	2012-08-16 01:32:09.000000000 +0100
@@ -22,6 +22,7 @@
 
 #include <sys/types.h>
 #include <sys/socket.h> /* For CMSG_* */
+#include <sys/time.h>
 
 #ifdef HAVE_LIMITS_H
 # include <limits.h> /* For PATH_MAX */
