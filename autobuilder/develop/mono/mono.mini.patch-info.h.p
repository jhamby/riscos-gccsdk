--- mono/mini/patch-info.h.orig	2011-08-09 02:18:44.000000000 +0100
+++ mono/mini/patch-info.h	2011-10-14 20:07:21.000000000 +0100
@@ -42,4 +42,5 @@
 PATCH_INFO(LLVM_IMT_TRAMPOLINE, "llvm_imt_trampoline")
 PATCH_INFO(GC_CARD_TABLE_ADDR, "gc_card_table_addr")
 PATCH_INFO(CASTCLASS_CACHE, "castclass_cache")
+PATCH_INFO(RISCOS_STKOVF, "riscos_stkovf")
 PATCH_INFO(NONE, "none")
