--- include/bfdlink.h.orig	2012-04-09 18:27:18.000000000 +0200
+++ include/bfdlink.h	2012-11-17 14:55:35.925613728 +0100
@@ -501,6 +501,9 @@ struct bfd_link_info
   /* Start and end of RELRO region.  */
   bfd_vma relro_start, relro_end;
 
+  /* 1 for -fpic, 2 for fPIC (RISC OS) */
+  unsigned int flag_pic;
+
   /* List of symbols should be dynamic.  */
   struct bfd_elf_dynamic_list *dynamic_list;
 
