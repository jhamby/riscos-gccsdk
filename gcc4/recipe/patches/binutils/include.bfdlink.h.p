--- include/bfdlink.h.orig	2009-09-10 13:47:30.000000000 +0200
+++ include/bfdlink.h	2010-01-31 16:32:36.212514900 +0100
@@ -465,6 +465,9 @@ struct bfd_link_info
   /* Start and end of RELRO region.  */
   bfd_vma relro_start, relro_end;
 
+  /* 1 for -fpic, 2 for fPIC (RISC OS) */
+  unsigned int flag_pic;
+
   /* List of symbols should be dynamic.  */
   struct bfd_elf_dynamic_list *dynamic_list;
 };
