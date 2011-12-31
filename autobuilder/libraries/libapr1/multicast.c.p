--- network_io/unix/multicast.c.orig	2011-12-29 12:07:11.000000000 +0000
+++ network_io/unix/multicast.c	2011-12-29 12:04:16.000000000 +0000
@@ -25,6 +25,9 @@
 #include <ifaddrs.h>
 #endif
 
+#define MCAST_JOIN_SOURCE_GROUP         46
+#define MCAST_LEAVE_SOURCE_GROUP        47
+
 #ifdef HAVE_STRUCT_IPMREQ
 static void fill_mip_v4(struct ip_mreq *mip, apr_sockaddr_t *mcast,
                         apr_sockaddr_t *iface)
