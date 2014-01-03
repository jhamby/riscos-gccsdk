--- include/bfdlink.h.orig	2013-11-04 16:33:39.000000000 +0100
+++ include/bfdlink.h	2013-12-31 13:16:36.478126417 +0100
@@ -524,6 +524,9 @@ struct bfd_link_info
   /* Start and end of RELRO region.  */
   bfd_vma relro_start, relro_end;
 
+  /* 1 for -fpic, 2 for fPIC (RISC OS) */
+  unsigned int flag_pic;
+
   /* List of symbols should be dynamic.  */
   struct bfd_elf_dynamic_list *dynamic_list;
 
