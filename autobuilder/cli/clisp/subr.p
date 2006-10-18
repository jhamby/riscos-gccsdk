--- src/subr_base.d	2006-05-18 13:44:51.000000000 +0100
+++ src/subr.d	2006-09-12 21:50:19.000000000 +0100
@@ -1419,3 +1419,8 @@
 LISPFUNN(current_process,0)
 LISPFUNN(list_processes,0)
 #endif
+/* ---------- RISCOS ---------- */
+LISPFUNN(riscos_swi,2)
+LISPFUNN(riscos_swix,2)
+LISPFUNNR(riscos_memoryref,1)
+LISPFUNN(riscos_memorystore,2)
