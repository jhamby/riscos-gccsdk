--- src/compress.c.old	2003-05-19 15:10:52.000000000 +0100
+++ src/compress.c	2003-05-19 14:09:22.000000000 +0100
@@ -366,7 +366,7 @@
 		    exit(0);
 		    /*NOTREACHED*/
 		case -1:
-		    error("could not fork (%s).\n", strerror(errno));
+		    file_error("could not fork (%s).\n", strerror(errno));
 		    /*NOTREACHED*/
 
 		/* default: // parent
