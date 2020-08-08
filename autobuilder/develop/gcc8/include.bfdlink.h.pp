--- include/bfdlink.h.orig	2018-01-13 13:31:16.000000000 +0000
+++ include/bfdlink.h	2019-01-14 22:15:56.609417942 +0000
@@ -635,6 +635,11 @@
 
   /* The version information.  */
   struct bfd_elf_version_tree *version_info;
+
+  /* 1 for -fpic, 2 for fPIC (RISC OS) */
+  unsigned int flag_pic;
+
+  const char *riscos_abi;
 };
 
 /* This structures holds a set of callback functions.  These are called
