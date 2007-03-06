--- bfd/bfd-in.h.orig	2007-03-04 03:49:52.000000000 +0100
+++ bfd/bfd-in.h	2007-02-05 20:20:43.000000000 +0100
@@ -847,6 +847,13 @@
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
 extern bfd_boolean bfd_is_arm_mapping_symbol_name
   (const char * name);
