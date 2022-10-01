--- src/net.c.orig	2022-09-25 10:56:45.520821916 +0100
+++ src/net.c	2022-09-25 10:58:23.164105962 +0100
@@ -24,7 +24,9 @@
 #include <arpa/inet.h>
 #include <sys/socket.h>
 #include <netinet/tcp.h>
+#ifndef __riscos__
 #include <ifaddrs.h>
+#endif
 #else
 #include <winsock2.h>
 #include <ws2tcpip.h>
@@ -600,7 +602,7 @@
 }
 
 
-#ifndef WIN32
+#if !defined WIN32 && !defined(__riscos__)
 static int net__bind_interface(struct mosquitto__listener *listener, struct addrinfo *rp)
 {
 	/*
@@ -740,7 +742,7 @@
 			return 1;
 		}
 
-#ifndef WIN32
+#if !defined WIN32 && !defined(__riscos__)
 		if(listener->bind_interface){
 			/* It might be possible that an interface does not support all relevant sa_families.
 			 * We should successfully find at least one. */
