--- src/constsym_base.d	2006-04-19 09:11:15.000000000 +0100
+++ src/constsym.d	2006-09-12 21:00:59.000000000 +0100
@@ -2052,3 +2059,8 @@
 LISPSYM(mem_write_vector,"MEM-WRITE-VECTOR",system)
 LISPSYM(affi_nonzerop,"NZERO-POINTER-P",system)
 #endif
+/* ---------- RISCOS ---------- */
+LISPSYM(riscos_swi,"SWI",riscos)
+LISPSYM(riscos_swix,"SWIX",riscos)
+LISPSYM(riscos_memoryref,"MEMORYREF",riscos)
+LISPSYM(riscos_memorystore,"MEMORYSTORE",riscos)
