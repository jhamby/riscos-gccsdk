--- vncviewer/sockets.cxx.orig	2005-06-01 14:37:05.000000000 +0100
+++ vncviewer/sockets.cxx	2005-06-01 14:37:14.000000000 +0100
@@ -266,40 +266,6 @@
 
 
 /*
- * FindFreeTcpPort tries to find unused TCP port in the range
- * (TUNNEL_PORT_OFFSET, TUNNEL_PORT_OFFSET + 99]. Returns 0 on failure.
- */
-
-int
-FindFreeTcpPort(void)
-{
-  int sock, port;
-  struct sockaddr_in addr;
-
-  addr.sin_family = AF_INET;
-  addr.sin_addr.s_addr = INADDR_ANY;
-
-  sock = socket(AF_INET, SOCK_STREAM, 0);
-  if (sock < 0) {
-    fprintf(stderr,programName);
-    perror(": FindFreeTcpPort: socket");
-    return 0;
-  }
-
-  for (port = TUNNEL_PORT_OFFSET + 99; port > TUNNEL_PORT_OFFSET; port--) {
-    addr.sin_port = htons((unsigned short)port);
-    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
-      close(sock);
-      return port;
-    }
-  }
-
-  close(sock);
-  return 0;
-}
-
-
-/*
  * ListenAtTcpPort starts listening at the given TCP port.
  */
 
