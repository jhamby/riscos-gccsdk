--- server/meta.c.old	2003-02-15 19:07:32.000000000 +0000
+++ server/meta.c	2003-02-15 19:12:54.000000000 +0000
@@ -228,14 +228,13 @@
     bad=true;
   }
 #else
-  /* IPv4 only
+  /* IPv4 only */
   bad = !fc_lookup_host(servername, &serv_addr);
   serv_addr.sin_port = htons(srvarg.metaserver_port);
-  */
   /* Dual-stack Compliant */
-  sockfd = net_connect_udp(servername, srvarg.metaserver_port);
+/*  sockfd = net_connect_udp(servername, srvarg.metaserver_port);
   if (sockfd < 0) {bad = 1;}
-  else bad = 0;
+  else bad = 0;*/
   /* End Dual-stack*/
 #endif
   if (bad) {
@@ -250,19 +249,19 @@
 #ifdef GENERATING_MAC  /* mac networking */
   meta_ep=OTOpenEndpoint(OTCreateConfiguration(kUDPName), 0, &meta_info, &err1);
   bad = (err1 != 0);
-/* IPv4 only
+ IPv4 only
 #else
   bad = ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1);
 
 #endif
-*/
+
   if (bad) {
     freelog(LOG_ERROR, "Metaserver: can't open datagram socket: %s",
 	    mystrerror(errno));
     metaserver_failed();
     return;
   }
-#endif
+/*#endif*/
   /*
    * Bind any local address for us and
    * associate datagram socket with server.
@@ -274,16 +273,15 @@
     metaserver_failed();
     return;
   }
-/* IPv4 only
-   #else */
+/* IPv4 only */
+   #else
   /* no, this is not weird, see man connect(2) --vasc */
-/* IPv4 only
+/* IPv4 only */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))==-1) {
     freelog(LOG_ERROR, "Metaserver: connect failed: %s", mystrerror(errno));
     metaserver_failed();
     return;
   }
- */
 #endif
 
   server_is_open = TRUE;
--- server/sernet.c.old	2003-02-15 19:13:31.000000000 +0000
+++ server/sernet.c	2003-02-15 19:20:48.000000000 +0000
@@ -693,11 +692,13 @@
   }
 /*  freelog(LOG_ERROR, "debugging - err: %d",err); */
 
+#if 0
   err = getnameinfo((struct sockaddr *) &fromend, fromlen, from, 1024, NULL, NULL, NULL);
   if (err == -1) {
     freelog(LOG_ERROR, "getnameinfo failed! %d %s", err, strerror(errno));
   }
-  
+#endif
+
 /*  freelog(LOG_ERROR, "debugging - sockfd  : %d ptr: %p len: %d", sockfd, fromend, fromlen); */
 
   /* End dual-stack */
@@ -727,10 +728,10 @@
 
       sz_strlcpy(pconn->name, makeup_connection_name(&pconn->id));
       sz_strlcpy(pconn->addr,
-                 /* IPv4 only
+                 /* IPv4 only */
                  (from ? from->h_name : inet_ntoa(fromend.sin_addr)));
-                 */
-                 from);
+                 /*
+                 from); */
       conn_list_insert_back(&game.all_connections, pconn);
   
       freelog(LOG_VERBOSE, "connection (%s) from %s", pconn->name, pconn->addr);
@@ -749,7 +750,7 @@
 {
   /* setup socket address */
 /* IPv4 Only */
-/*
+
     struct sockaddr_in src;
   int opt;
 
@@ -778,9 +779,9 @@
     freelog(LOG_FATAL, "listen failed: %s", mystrerror(errno));
     exit(EXIT_FAILURE);
   }
-  */
   /* Dual-stack-compliant */
   /* setup socket address */
+#if 0
 	int n;
 	const int on = 1;
 	struct addrinfo	hints, *res, *ressave;
@@ -837,6 +838,7 @@
 	freeaddrinfo(ressave);
 
   close_socket_set_callback(close_socket_callback);
+#endif
   return 0;
 }
 
--- server/meta.c	2003-02-15 19:55:14.000000000 +0000
+++ server/meta.c.old	2003-02-15 19:55:04.000000000 +0000
@@ -208,9 +208,8 @@
   InetSvcRef ref=OTOpenInternetServices(kDefaultInternetServicesPath, 0, &err1);
   InetHostInfo hinfo;
 #else
+  /* IPv4 only */
-  /* IPv4 only
   struct sockaddr_in serv_addr;
-   */
 #endif
   
   /*
