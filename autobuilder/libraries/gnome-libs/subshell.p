--- zvt/subshell.c.old	2003-04-24 22:25:52.000000000 +0100
+++ zvt/subshell.c	2003-04-24 21:27:54.000000000 +0100
@@ -185,12 +185,12 @@
 static int
 receive_fd (int helper_fd)
 {
-	struct strrecvfd recvfd;
+//	struct strrecvfd recvfd;
 	
-	if (ioctl (helper_fd, I_RECVFD, &recvfd) < 0)
+//	if (ioctl (helper_fd, I_RECVFD, &recvfd) < 0)
 		return -1;
 
-	return recvfd.fd;
+//	return recvfd.fd;
 }
 
 static int
