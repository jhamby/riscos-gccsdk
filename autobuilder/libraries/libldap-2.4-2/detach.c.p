--- libraries/liblutil/detach.c.org	2006-05-12 22:34:30.060000000 +0200
+++ libraries/liblutil/detach.c	2006-05-12 22:34:47.520000000 +0200
@@ -128,7 +128,7 @@
 
 #ifdef HAVE_SETSID
 		(void) setsid();
-#elif TIOCNOTTY
+#elif defined(TIOCNOTTY)
 		if ( (sd = open( "/dev/tty", O_RDWR )) != -1 ) {
 			(void) ioctl( sd, TIOCNOTTY, NULL );
 			(void) close( sd );
