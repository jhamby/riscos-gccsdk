--- src/network/udp.c.orig	2009-08-28 09:00:07.000000000 -0700
+++ src/network/udp.c	2009-08-28 09:04:28.000000000 -0700
@@ -57,6 +57,10 @@
 #   endif
 #endif
 
+#ifdef __riscos__
+#   define if_nametoindex( str ) atoi( str )
+#endif
+
 #ifdef HAVE_LINUX_DCCP_H
 # include <linux/dccp.h>
 # ifndef SOCK_DCCP /* provisional API */
