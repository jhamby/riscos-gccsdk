--- bfd/bfd-in.h.orig	2011-09-16 03:15:18.000000000 +0200
+++ bfd/bfd-in.h	2011-11-21 19:42:17.402252956 +0100
@@ -881,7 +881,20 @@ extern bfd_boolean bfd_elf32_arm_get_bfd
 extern bfd_boolean bfd_elf32_arm_add_glue_sections_to_bfd
   (bfd *, struct bfd_link_info *);
 
-/* ELF ARM mapping symbol support */
+/* ELF ARM RISC OS module support. Called from linker.  */
+extern bfd_boolean bfd_elf32_arm_create_riscos_module_reloc
+  (bfd *, struct bfd_link_info *);
+
+extern bfd_boolean bfd_elf32_arm_allocate_riscos_module_reloc
+  (struct bfd_link_info *);
+
+extern bfd_boolean bfd_elf32_arm_create_riscos_pic_section
+  (struct bfd_link_info *);
+
+extern bfd_boolean bfd_elf32_arm_allocate_riscos_pic_section
+  (struct bfd_link_info *);
+
+  /* ELF ARM mapping symbol support */
 #define BFD_ARM_SPECIAL_SYM_TYPE_MAP	(1 << 0)
 #define BFD_ARM_SPECIAL_SYM_TYPE_TAG	(1 << 1)
 #define BFD_ARM_SPECIAL_SYM_TYPE_OTHER  (1 << 2)
