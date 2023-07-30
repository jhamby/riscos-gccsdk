--- lib/net_mosq.c.orig	2022-09-25 10:36:27.887351415 +0100
+++ lib/net_mosq.c	2022-09-25 10:36:53.759059650 +0100
@@ -964,7 +964,7 @@
 	}
 	else {
 		net__print_ssl_error(mosq);
-		errno = EPROTO;
+		errno = EPROTONOSUPPORT;
 	}
 	ERR_clear_error();
 #ifdef WIN32
