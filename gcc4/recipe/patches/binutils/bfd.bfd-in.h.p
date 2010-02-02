--- bfd/bfd-in.h.orig	2009-09-02 09:18:35.000000000 +0200
+++ bfd/bfd-in.h	2010-01-31 16:19:42.005013604 +0100
@@ -873,6 +873,13 @@ extern bfd_boolean bfd_elf32_arm_get_bfd
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
