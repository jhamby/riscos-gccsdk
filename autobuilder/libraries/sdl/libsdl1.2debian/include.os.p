--- src/video/riscos/SDL_wimppoll.c.orig	2008-04-12 15:27:52.000000000 -0700
+++ src/video/riscos/SDL_wimppoll.c	2008-04-12 15:28:05.000000000 -0700
@@ -43,7 +43,6 @@
 
 #include "kernel.h"
 #include "swis.h"
-#include "unixlib/os.h"
 
 #if !SDL_THREADS_DISABLED
 #include <pthread.h>
--- src/video/riscos/SDL_riscosFullScreenVideo.c.orig	2008-04-12 15:26:54.000000000 -0700
+++ src/video/riscos/SDL_riscosFullScreenVideo.c	2008-04-12 15:27:09.000000000 -0700
@@ -41,7 +41,6 @@
 
 #include "kernel.h"
 #include "swis.h"
-#include "unixlib/os.h"
 #include "unixlib/local.h"
 
 /* Private structures */
