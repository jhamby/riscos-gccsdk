--- Modules/posixmodule.c.orig	2023-07-01 11:34:50.776309626 +0100
+++ Modules/posixmodule.c	2023-07-01 11:35:31.239585993 +0100
@@ -3083,10 +3083,14 @@
 /*[clinic end generated code: output=c424d2e9d1cd636a input=9ff5a58b08115c55]*/
 {
 
+#ifndef __riscos__
     long size = sysconf(_SC_TTY_NAME_MAX);
     if (size == -1) {
         return posix_error();
     }
+#else
+    long size = 255;
+#endif
     char *buffer = (char *)PyMem_RawMalloc(size);
     if (buffer == NULL) {
         return PyErr_NoMemory();
