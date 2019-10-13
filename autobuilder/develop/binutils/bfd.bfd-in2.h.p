--- bfd/bfd-in2.h.orig	2019-01-14 23:35:16.000000000 +0000
+++ bfd/bfd-in2.h	2019-01-14 23:40:55.101564158 +0000
@@ -925,6 +925,7 @@
   int merge_exidx_entries;
   int cmse_implib;
   bfd *in_implib_bfd;
+  int riscos_module;
 };
 
 void bfd_elf32_arm_set_target_params
