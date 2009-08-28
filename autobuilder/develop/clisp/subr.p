--- src/subr.d.orig	2006-10-06 00:32:16.000000000 +0100
+++ src/subr.d	2006-10-22 15:10:11.000000000 +0100
@@ -1424,3 +1424,10 @@
 LISPFUNN(current_process,0)
 LISPFUNN(list_processes,0)
 #endif
+/* ---------- RISCOS ---------- */
+LISPFUNN(riscos_swi,2)
+LISPFUNN(riscos_swix,2)
+LISPFUNN(riscos_malloc,1)
+LISPFUNN(riscos_free,1)
+LISPFUNNR(riscos_memoryref,1)
+LISPFUNN(riscos_memorystore,2)
