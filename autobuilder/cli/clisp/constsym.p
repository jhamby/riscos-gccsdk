--- src/constsym.d.orig	2006-10-06 00:32:16.000000000 +0100
+++ src/constsym.d	2006-10-22 19:47:15.000000000 +0100
@@ -2057,3 +2057,10 @@
 LISPSYM(mem_write_vector,"MEM-WRITE-VECTOR",system)
 LISPSYM(affi_nonzerop,"NZERO-POINTER-P",system)
 #endif
+/* ---------- RISCOS ---------- */
+LISPSYM(riscos_swi,"SWI",riscos)
+LISPSYM(riscos_swix,"SWIX",riscos)
+LISPSYM(riscos_malloc,"MALLOC",riscos)
+LISPSYM(riscos_free,"FREE",riscos)
+LISPSYM(riscos_memoryref,"MEMORYREF",riscos)
+LISPSYM(riscos_memorystore,"RISCOS-MEMORYSTORE",system)
