Index: gcc/config/arm/elf.h
===================================================================
--- gcc/config/arm/elf.h	(revision 178747)
+++ gcc/config/arm/elf.h	(working copy)
@@ -128,8 +128,12 @@
 #define TARGET_ASM_DESTRUCTOR arm_elf_asm_destructor
 
 /* For PIC code we need to explicitly specify (PLT) and (GOT) relocs.  */
+#ifndef NEED_PLT_RELOC
 #define NEED_PLT_RELOC	flag_pic
+#endif
+#ifndef NEED_GOT_RELOC
 #define NEED_GOT_RELOC	flag_pic
+#endif
 
 /* The ELF assembler handles GOT addressing differently to NetBSD.  */
 #define GOT_PCREL	0
