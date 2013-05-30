--- activation-server/Makefile.in.orig	2013-05-29 21:06:19.906442010 +0100
+++ activation-server/Makefile.in	2013-05-29 21:06:43.122406126 +0100
@@ -341,7 +341,6 @@
 	$(SERVER_CFLAGS)			\
 	$(WARN_CFLAGS)				\
 	-DSERVER_CONFDIR=\"$(sysconfdir)\"	\
-	-DG_DISABLE_DEPRECATED			\
 	-DG_LOG_DOMAIN=\"Bonobo-Activation-Server\" \
 	$(NULL)
 
