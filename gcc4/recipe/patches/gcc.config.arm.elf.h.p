--- gcc/config/arm/elf.h.orig	2008-12-13 17:09:43.000000000 +0100
+++ gcc/config/arm/elf.h	2008-12-13 17:09:02.000000000 +0100
@@ -127,8 +127,12 @@
 #define TARGET_ASM_CONSTRUCTOR arm_elf_asm_constructor
 
 /* For PIC code we need to explicitly specify (PLT) and (GOT) relocs.  */
+#ifndef NEED_PLT_RELOC
 #define NEED_PLT_RELOC	flag_pic
+#endif
+#ifndef NEED_GOT_RELOC
 #define NEED_GOT_RELOC	flag_pic
+#endif
 
 /* The ELF assembler handles GOT addressing differently to NetBSD.  */
 #define GOT_PCREL	0
