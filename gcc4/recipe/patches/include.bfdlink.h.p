--- include/bfdlink.h.orig	2006-10-19 16:38:56.000000000 +0100
+++ include/bfdlink.h	2006-10-18 20:40:38.000000000 +0100
@@ -436,6 +436,9 @@
 
   /* List of symbols should be dynamic.  */
   struct bfd_elf_dynamic_list *dynamic;
+
+  /* 1 for -fpic, 2 for -fPIC (RISC OS) */
+  unsigned int flag_pic;
 };
 
 /* This structures holds a set of callback functions.  These are
