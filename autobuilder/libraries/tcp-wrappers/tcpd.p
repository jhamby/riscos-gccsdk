--- tcpd.h.old	2004-09-13 15:14:11.000000000 +0100
+++ tcpd.h	2004-09-13 15:19:52.000000000 +0100
@@ -83,7 +83,7 @@
 extern void shell_cmd(char *);		/* execute shell command */
 extern char *percent_x(char *, int, char *, struct request_info *);
 					/* do %<char> expansion */
-extern void rfc931(struct sockaddr *, struct sockaddr *, char *);
+extern void rfc931(struct sockaddr_in *, struct sockaddr_in *, char *);
 					/* client name from RFC 931 daemon */
 extern void clean_exit(struct request_info *);	/* clean up and exit */
 extern void refuse(struct request_info *);	/* clean up and exit */
