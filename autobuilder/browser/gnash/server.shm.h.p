--- server/shm.h	31 Oct 2006 09:44:55 -0000	1.11
+++ server/shm.h	18 Nov 2006 17:34:09 -0000
@@ -29,10 +29,10 @@
 
 #include "impl.h"
 #include <sys/types.h>
-#ifndef HAVE_WINSOCK_H
+#if !defined(HAVE_WINSOCK_H) && !defined(__riscos__)
 # include <sys/ipc.h>
 # include <sys/shm.h>
-#else
+#elif !defined(__riscos__)
 # include <windows.h>
 # include <process.h>
 # include <fcntl.h>
