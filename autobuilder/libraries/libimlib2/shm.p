--- src/lib/grab.c.orig	2009-07-26 18:26:38.000000000 -0700
+++ src/lib/grab.c	2009-07-26 18:28:17.000000000 -0700
@@ -4,8 +4,8 @@
 #include <X11/extensions/XShm.h>
 #include <X11/Xutil.h>
 #include <X11/extensions/shape.h>
-#include <sys/ipc.h>
-#include <sys/shm.h>
+//#include <sys/ipc.h>
+//#include <sys/shm.h>
 #include "grab.h"
 #include "ximage.h"
 
--- src/lib/ximage.c.orig	2009-07-26 18:33:27.000000000 -0700
+++ src/lib/ximage.c	2009-07-26 18:33:40.000000000 -0700
@@ -3,7 +3,7 @@
 #include <X11/Xlib.h>
 #include <X11/extensions/XShm.h>
 #include <X11/Xutil.h>
-#include <sys/ipc.h>
-#include <sys/shm.h>
+//#include <sys/ipc.h>
+//#include <sys/shm.h>
 #include "ximage.h"
 
@@ -56,14 +56,14 @@
       return NULL;
 
    /* get an shm id of this image */
-   si->shmid = shmget(IPC_PRIVATE, xim->bytes_per_line * xim->height,
-                      IPC_CREAT | 0666);
+/*   si->shmid = shmget(IPC_PRIVATE, xim->bytes_per_line * xim->height,
+                      IPC_CREAT | 0666); */
    /* if the get succeeds */
    if (si->shmid != -1)
      {
         /* set the params for the shm segment */
         si->readOnly = False;
-        si->shmaddr = xim->data = shmat(si->shmid, 0, 0);
+/*        si->shmaddr = xim->data = shmat(si->shmid, 0, 0); */
         /* get the shm addr for this data chunk */
         if (xim->data != (char *)-1)
           {
@@ -98,12 +98,12 @@
              x_does_shm = 0;
 
              /* detach */
-             shmdt(si->shmaddr);
+/*             shmdt(si->shmaddr); */
           }
 
         /* get failed - out of shm id's or shm segment too big ? */
         /* remove the shm id we created */
-        shmctl(si->shmid, IPC_RMID, 0);
+/*        shmctl(si->shmid, IPC_RMID, 0); */
      }
 
    /* couldnt create SHM image ? */
@@ -118,8 +118,8 @@
 {
    XSync(d, False);
    XShmDetach(d, si);
-   shmdt(si->shmaddr);
-   shmctl(si->shmid, IPC_RMID, 0);
+/*   shmdt(si->shmaddr);
+   shmctl(si->shmid, IPC_RMID, 0); */
 }
 
 /* "safe" realloc allowing handling of out-of-memory situations */
