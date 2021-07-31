--- linc2/src/linc-protocols.c.orig	2009-07-20 13:18:19.000000000 -0700
+++ linc2/src/linc-protocols.c	2009-07-20 13:20:38.000000000 -0700
@@ -139,7 +139,7 @@
 
 #if defined(AF_INET) || defined(AF_INET6) || defined (AF_UNIX)
 
-#ifndef G_OS_WIN32
+#if !defined(G_OS_WIN32) && !defined(__riscos__)
 static void
 get_first_non_local_ipaddr(char *buf, 
 			   size_t len)
@@ -198,8 +198,10 @@
 #ifndef G_OS_WIN32
 		if (fixed_host_net_id) 
 			strncpy(buf, fixed_host_net_id, len);
+#if 0 
 		else
 			get_first_non_local_ipaddr(buf, len);
+#endif
 		if ('\0' == *buf)
 			strncpy(buf, "127.0.0.1", len);
 
@@ -608,9 +610,11 @@
 
 		LINK_RESOLV_UNSET_IPV6;
 #ifdef HAVE_RESOLV_H
+#if 0
 		if (!(_res.options & RES_INIT))
 			res_init();
 #endif
+#endif
 		
                 d_printf("%s:%s:%d:gethostbyname(%s)\n", __FILE__, __FUNCTION__, __LINE__, hostname);
 		host = gethostbyname (hostname);
