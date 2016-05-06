--- ssl/s2_meth.c.orig	2016-03-01 13:35:05.000000000 +0000
+++ ssl/s2_meth.c	2016-05-05 13:00:14.274912616 +0100
@@ -78,4 +78,9 @@
 static void *dummy = &dummy;
 # endif
 
+/* Fix to allow libcurl3 to link statically - may be included in future libssl */
+SSL_METHOD *SSLv2_method(void) { return NULL; }
+SSL_METHOD *SSLv2_client_method(void) { return NULL; }
+SSL_METHOD *SSLv2_server_method(void) { return NULL; }
+
 #endif
