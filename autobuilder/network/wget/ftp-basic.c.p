--- src/ftp-basic.c.orig  2015-11-16 21:44:24.000000000 +0200
+++ src/ftp-basic.c   2015-11-16 21:46:29.954077209 +0200
@@ -428,6 +428,7 @@
   snprintf (buf, buflen, "|%d|%s|%d|", afnum, print_address (addr), port);
   buf[buflen - 1] = '\0';
 }
+#endif /* ENABLE_IPV6 */
 
 #ifdef HAVE_SSL
 /*
@@ -542,6 +543,7 @@
 }
 #endif /* HAVE_SSL */
 
+#ifdef ENABLE_IPV6
 /* Bind a port and send the appropriate PORT command to the FTP
    server.  Use acceptport after RETR, to get the socket of data
    connection.  */
@@ -599,7 +601,7 @@
   xfree (respline);
   return FTPOK;
 }
-#endif
+#endif /* ENABLE_IPV6 */
 
 /* Similar to ftp_port, but uses `PASV' to initiate the passive FTP
    transfer.  Reads the response from server and parses it.  Reads the

