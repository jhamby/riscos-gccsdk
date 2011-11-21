--- include/bfdlink.h.orig	2011-11-21 10:29:37.000000000 +0100
+++ include/bfdlink.h	2011-11-21 19:42:18.678236913 +0100
@@ -498,6 +498,9 @@ struct bfd_link_info
   /* Start and end of RELRO region.  */
   bfd_vma relro_start, relro_end;
 
+  /* 1 for -fpic, 2 for fPIC (RISC OS) */
+  unsigned int flag_pic;
+
   /* List of symbols should be dynamic.  */
   struct bfd_elf_dynamic_list *dynamic_list;
 
