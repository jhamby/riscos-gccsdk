--- libusal/scsi-remote.c.orig	2020-12-09 18:45:26.651479331 +0000
+++ libusal/scsi-remote.c	2020-12-09 18:47:15.313330517 +0000
@@ -574,6 +574,7 @@
 					pw->pw_name, name, rscsi, rsh);
 	else
 #endif
+#ifndef __riscos__
 #ifdef	HAVE_RCMD
 		rscsisock = rcmd(&rscsipeer, (unsigned short)sp->s_port,
 					pw->pw_name, name, rscsi, 0);
@@ -581,7 +582,7 @@
 		rscsisock = _rcmdrsh(&rscsipeer, (unsigned short)sp->s_port,
 					pw->pw_name, name, rscsi, rsh);
 #endif
-
+#endif
 	return (rscsisock);
 }
 
