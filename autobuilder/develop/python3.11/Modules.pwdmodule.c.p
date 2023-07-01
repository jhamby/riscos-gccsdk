--- Modules/pwdmodule.c.orig	2023-07-01 11:38:12.777109434 +0100
+++ Modules/pwdmodule.c	2023-07-01 11:39:51.436205264 +0100
@@ -146,7 +146,11 @@
     struct passwd pwd;
 
     Py_BEGIN_ALLOW_THREADS
+#ifndef __riscos__
     bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
+#else
+    bufsize = -1;
+#endif
     if (bufsize == -1) {
         bufsize = DEFAULT_BUFFER_SIZE;
     }
@@ -229,7 +233,11 @@
     struct passwd pwd;
 
     Py_BEGIN_ALLOW_THREADS
+#ifndef __riscos__
     bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
+#else
+    bufsize = -1;
+#endif
     if (bufsize == -1) {
         bufsize = DEFAULT_BUFFER_SIZE;
     }
