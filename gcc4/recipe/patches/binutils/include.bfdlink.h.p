--- include/bfdlink.h.orig	2011-06-27 09:39:13.000000000 +0200
+++ include/bfdlink.h	2011-06-27 22:54:09.594438173 +0200
@@ -466,6 +466,9 @@ struct bfd_link_info
   /* Start and end of RELRO region.  */
   bfd_vma relro_start, relro_end;
 
+  /* 1 for -fpic, 2 for fPIC (RISC OS) */
+  unsigned int flag_pic;
+
   /* List of symbols should be dynamic.  */
   struct bfd_elf_dynamic_list *dynamic_list;
 };
