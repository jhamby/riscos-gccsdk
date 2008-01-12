--- player/main.c.orig	2008-01-12 11:37:45.000000000 -0800
+++ player/main.c	2008-01-12 11:51:15.000000000 -0800
@@ -26,8 +26,8 @@
 #include <sys/time.h>
 #include <sys/types.h>
 #include <unistd.h>
-#include <sys/ipc.h>
-#include <sys/shm.h>
+//#include <sys/ipc.h>
+//#include <sys/shm.h>
 #include "flash.h"
 #include <X11/Xlib.h>
 #include <X11/keysym.h>
@@ -113,7 +113,7 @@
     Visual *visual;
 /*    int nItems;*/
     int n;
-    struct shmid_ds buf;
+    //    struct shmid_ds buf;
     int			 	 targetWidth;
     int 			 targetHeight;
     long			 bpl;	// Bytes per line
@@ -197,16 +197,20 @@
 
 	// Prepare data for Direct Graphics
 	segInfo.readOnly = False;
-	segInfo.shmid = shmget (IPC_PRIVATE,targetHeight*bpl,IPC_CREAT|0777);
-	if (segInfo.shmid <0) {
-		perror("shmget");
-		fprintf(stderr,"Size = %d x %d\n", targetWidth, targetHeight);
-	}
-	segInfo.shmaddr = (char*)shmat (segInfo.shmid, 0, 0);
-	if ((long)segInfo.shmaddr == -1) {
-		perror("shmat");
-	}
-	XShmAttach(dpy, &segInfo);
+	//segInfo.shmid =  shmget (IPC_PRIVATE,targetHeight*bpl,IPC_CREAT|0777);
+	//if (segInfo.shmid <0) {
+	//	perror("shmget");
+	//	fprintf(stderr,"Size = %d x %d\n", targetWidth, targetHeight);
+	//}
+	segInfo.shmaddr = malloc(targetHeight*bpl); // ( (char*)shmat (segInfo.shmid, 0, 0);
+	//if ((long)segInfo.shmaddr == -1) {
+	//	perror("shmat");
+	//}
+        if (!segInfo.shmaddr) {
+	        fputs("malloc failed\n", stderr);
+                exit(1);
+        }
+	//XShmAttach(dpy, &segInfo);
 #ifdef linux
 	// Warning : this does NOT work properly on Solaris
 	// Special Linux shm behaviour is used here
