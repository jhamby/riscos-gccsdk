--- libretro-common/net/net_ifinfo.c.orig	2018-11-09 17:54:43.371233434 +0000
+++ libretro-common/net/net_ifinfo.c	2018-11-09 17:56:49.400670597 +0000
@@ -35,6 +35,7 @@
 #include <sys/socket.h>
 #include <netdb.h>
 
+#ifndef __riscos__
 #ifdef WANT_IFADDRS
 #include <compat/ifaddrs.h>
 #else
@@ -43,6 +44,7 @@
 #endif
 #endif
 #endif
+#endif
 
 #include <net/net_ifinfo.h>
 
@@ -91,6 +93,7 @@
 
 bool net_ifinfo_new(net_ifinfo_t *list)
 {
+#ifndef __riscos__
    unsigned k              = 0;
 #ifdef HAVE_LIBNX
    uint32_t id;
@@ -246,6 +249,6 @@
    freeifaddrs(ifaddr);
 #endif
    net_ifinfo_free(list);
-
+#endif
    return false;
 }
