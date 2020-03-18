--- include/bfdlink.h.orig	2020-03-16 18:08:18.149473777 +0000
+++ include/bfdlink.h	2020-03-16 18:04:54.139390026 +0000
@@ -529,6 +529,11 @@ struct bfd_link_info
 
   /* The version information.  */
   struct bfd_elf_version_tree *version_info;
+
+  /* 1 for -fpic, 2 for fPIC (RISC OS) */
+  unsigned int flag_pic;
+
+  const char *riscos_abi;
 };
 
 /* This structures holds a set of callback functions.  These are called
