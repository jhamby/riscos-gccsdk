--- bfd/bfd-in.h.orig	2018-01-13 13:31:15.000000000 +0000
+++ bfd/bfd-in.h	2019-01-17 22:13:33.544924940 +0000
@@ -918,6 +918,9 @@
   int merge_exidx_entries;
   int cmse_implib;
   bfd *in_implib_bfd;
+#ifdef __RISCOS_TARGET__
+  int riscos_module;
+#endif
 };
 
 void bfd_elf32_arm_set_target_params
@@ -932,6 +935,19 @@
 extern void bfd_elf32_arm_keep_private_stub_output_sections
   (struct bfd_link_info *);
 
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
 /* ELF ARM mapping symbol support.  */
 #define BFD_ARM_SPECIAL_SYM_TYPE_MAP	(1 << 0)
 #define BFD_ARM_SPECIAL_SYM_TYPE_TAG	(1 << 1)
