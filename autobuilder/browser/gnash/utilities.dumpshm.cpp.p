--- utilities/dumpshm.cpp	29 Oct 2006 18:34:51 -0000	1.8
+++ utilities/dumpshm.cpp	18 Nov 2006 17:34:12 -0000
@@ -39,11 +39,11 @@
 }
 #include <dirent.h>
 #include <sys/types.h>
-#ifndef HAVE_WINSOCK_H
+#if !defined(HAVE_WINSOCK_H) && !defined(__riscos__)
 #include <sys/mman.h>
 #include <sys/shm.h>
 #include <sys/ipc.h>
-#else
+#elif !defined(__riscos__)
 #include <windows.h>
 #include <process.h>
 #include <io.h>
