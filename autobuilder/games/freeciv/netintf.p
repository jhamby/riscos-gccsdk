--- common/netintf.c.old	2003-02-15 04:30:28.000000000 +0000
+++ common/netintf.c	2003-02-15 19:03:16.000000000 +0000
@@ -165,7 +165,7 @@
   Look up the given host and fill in *sock.  Note that the caller
   should fill in the port number (sock->sin_port).
 ***************************************************************************/
-/* IPv4 Only
+/* IPv4 Only */
 bool fc_lookup_host(const char *hostname, struct sockaddr_in *sock)
 {
   struct hostent *hp;
@@ -189,7 +189,7 @@
   memcpy(&sock->sin_addr, hp->h_addr, hp->h_length);
   return TRUE;
   }
-*/
+#if 0
 /* Dual-stack compliant */
 bool fc_lookup_host(const char *hostname, int port)
 {
@@ -272,3 +272,4 @@
 	return(sockfd);
 }
 /* End dual-stack */
+#endif
--- common/netintf.h.old	2003-02-15 19:05:00.000000000 +0000
+++ common/netintf.h	2003-02-15 19:05:08.000000000 +0000
@@ -55,10 +55,11 @@
 void my_shutdown_network(void);
 
 void my_nonblock(int sockfd);
-/*IPv4 only
+/*IPv4 only*/
 bool fc_lookup_host(const char *hostname, struct sockaddr_in *sock);
-*/
+/*
 bool fc_lookup_host(const char *hostname, int port);
+*/
 /* End dual-stack*/
 
 #endif  /* FC__NETINTF_H */
