--- src/lib/grab.c.orig	2009-07-26 18:26:38.000000000 -0700
+++ src/lib/grab.c	2009-07-26 18:28:17.000000000 -0700
@@ -4,8 +4,8 @@
 # include <X11/extensions/XShm.h>
 # include <X11/Xutil.h>
 # include <X11/extensions/shape.h>
-# include <sys/ipc.h>
-# include <sys/shm.h>
+//# include <sys/ipc.h>
+//# include <sys/shm.h>
 #endif
 #include "grab.h"
 
@@ -674,6 +674,7 @@
 
    prev_erh = XSetErrorHandler((XErrorHandler) Tmp_HandleXError);
 
+#if 0
    if (x_does_shm)
      {
         _x_err = 0;
@@ -720,6 +721,7 @@
                }
           }
      }
+#endif
    if (!is_shm)
       xim = XGetImage(d, p, x, y, w, h, 0xffffffff, ZPixmap);
    if (!xim)
@@ -732,6 +734,7 @@
    if ((m) && (domask))
      {
         _x_err = 0;
+#if 0
         if (x_does_shm)
           {
              mxim = XShmCreateImage(d, v, 1, ZPixmap, NULL, &mshminfo, w, h);
@@ -779,6 +782,7 @@
                     }
                }
           }
+#endif
         if (!is_mshm)
            mxim = XGetImage(d, m, 0, 0, w, h, 0xffffffff, ZPixmap);
      }
@@ -837,6 +841,7 @@
                             d, xim, mxim, v, xatt.depth, x, y, w, h, 0);
 
    /* destroy the Ximage */
+#if 0
    if (is_shm)
      {
         XSync(d, False);
@@ -850,6 +855,7 @@
         shmdt(mshminfo.shmaddr);
         shmctl(mshminfo.shmid, IPC_RMID, 0);
      }
+#endif
    XDestroyImage(xim);
    if (created_mask)
       XFreePixmap(d, m);
--- src/lib/ximage.c.orig	2009-07-26 18:33:27.000000000 -0700
+++ src/lib/ximage.c	2009-07-26 18:33:40.000000000 -0700
@@ -3,8 +3,8 @@
 # include <X11/Xlib.h>
 # include <X11/extensions/XShm.h>
 # include <X11/Xutil.h>
-# include <sys/ipc.h>
-# include <sys/shm.h>
+//# include <sys/ipc.h>
+//# include <sys/shm.h>
 # include "ximage.h"
 #endif
 
@@ -102,8 +102,8 @@
                   XDestroyImage(xim);
                   if (list_si[i])
                     {
-                       shmdt(list_si[i]->shmaddr);
-                       shmctl(list_si[i]->shmid, IPC_RMID, 0);
+                       //shmdt(list_si[i]->shmaddr);
+                       //shmctl(list_si[i]->shmid, IPC_RMID, 0);
                        free(list_si[i]);
                     }
                   list_num--;
@@ -249,16 +249,16 @@
              /* add to list */
              list_xim[list_num - 1] = xim;
              /* get an shm id of this image */
-             list_si[list_num - 1]->shmid =
-                shmget(IPC_PRIVATE, xim->bytes_per_line * xim->height,
-                       IPC_CREAT | 0777);
+             list_si[list_num - 1]->shmid = -1;
+                //shmget(IPC_PRIVATE, xim->bytes_per_line * xim->height,
+                //       IPC_CREAT | 0777);
              /* if the get succeeds */
              if (list_si[list_num - 1]->shmid != -1)
                {
                   /* set the params for the shm segment */
                   list_si[list_num - 1]->readOnly = False;
-                  list_si[list_num - 1]->shmaddr = xim->data =
-                     shmat(list_si[list_num - 1]->shmid, 0, 0);
+                  list_si[list_num - 1]->shmaddr = xim->data = -1;
+                     //shmat(list_si[list_num - 1]->shmid, 0, 0);
                   /* get the shm addr for this data chunk */
                   if (xim->data != (char *)-1)
                     {
@@ -291,9 +291,9 @@
                             /* destroy our previous image */
                             XDestroyImage(xim);
                             /* detach */
-                            shmdt(list_si[list_num - 1]->shmaddr);
+                            //shmdt(list_si[list_num - 1]->shmaddr);
                             /* remove the shm id */
-                            shmctl(list_si[list_num - 1]->shmid, IPC_RMID, 0);
+                            //shmctl(list_si[list_num - 1]->shmid, IPC_RMID, 0);
                             /* flag out xim as NULL */
                             xim = NULL;
                          }
@@ -304,7 +304,7 @@
                        /* destroy previous image */
                        XDestroyImage(xim);
                        /* remove the shm id we created */
-                       shmctl(list_si[list_num - 1]->shmid, IPC_RMID, 0);
+                       //shmctl(list_si[list_num - 1]->shmid, IPC_RMID, 0);
                        /* flag xim as NULL */
                        xim = NULL;
                     }
