--- bfd/bfd-in.h.orig	2011-03-16 09:35:44.000000000 +0100
+++ bfd/bfd-in.h	2011-06-27 22:54:09.564435191 +0200
@@ -877,6 +877,13 @@ extern bfd_boolean bfd_elf32_arm_get_bfd
 extern bfd_boolean bfd_elf32_arm_add_glue_sections_to_bfd
   (bfd *, struct bfd_link_info *);
 
+/* ELF ARM RISC OS module support. Called from linker.  */
+extern bfd_boolean bfd_elf32_arm_create_riscos_module_reloc
+  (bfd *, struct bfd_link_info *);
+
+extern bfd_boolean bfd_elf32_arm_allocate_riscos_module_reloc
+  (struct bfd_link_info *);
+
 /* ELF ARM mapping symbol support */
 #define BFD_ARM_SPECIAL_SYM_TYPE_MAP	(1 << 0)
 #define BFD_ARM_SPECIAL_SYM_TYPE_TAG	(1 << 1)
