--- lib/loop.c.orig	2022-09-25 10:29:40.016952752 +0100
+++ lib/loop.c	2022-09-25 10:34:59.308396122 +0100
@@ -285,7 +285,7 @@
 			case MOSQ_ERR_ERRNO:
 				break;
 		}
-		if(errno == EPROTO){
+		if(errno == EPROTONOSUPPORT){
 			return rc;
 		}
 		do{
