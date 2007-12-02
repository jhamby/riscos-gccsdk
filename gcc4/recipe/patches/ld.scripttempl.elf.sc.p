--- ld/scripttempl/elf.sc.orig	2007-12-02 20:17:58.000000000 +0100
+++ ld/scripttempl/elf.sc	2007-12-02 20:08:49.000000000 +0100
@@ -236,6 +236,21 @@
    test -z "${TEXT_BASE_ADDRESS}" && TEXT_BASE_ADDRESS="${TEXT_START_ADDR}"
 fi
 
+# These are the standard RISC OS linker variables
+RISCOS_ROBASE="PROVIDE (Image\$\$RO\$\$Base = .);"
+RISCOS_ROLIMIT="PROVIDE (Image\$\$RO\$\$Limit = .);"
+RISCOS_RWBASE="PROVIDE (Image\$\$RW\$\$Base = .);"
+RISCOS_RWLIMIT="PROVIDE (Image\$\$RW\$\$Limit = .);"
+RISCOS_ZIBASE="PROVIDE (Image\$\$ZI\$\$Base = .);"
+RISCOS_ZILIMIT="PROVIDE (Image\$\$ZI\$\$Limit = .);"
+
+RISCOS_IMAGE_HDR="
+  /* Make space for run-time data when dynamically linked. */
+  . = . + 40;"
+RISCOS_SOM_GOT_SYMBOL="
+  /* Define a symbol that allows the RISC OS dynamic loader to find the GOT. */
+  __som_got__ = .;"
+
 cat <<EOF
 OUTPUT_FORMAT("${OUTPUT_FORMAT}", "${BIG_OUTPUT_FORMAT}",
 	      "${LITTLE_OUTPUT_FORMAT}")
@@ -256,6 +271,8 @@
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+PROVIDE (__executable_start = ${TEXT_START_ADDR}); . = ${TEXT_BASE_ADDRESS};}}}
   ${CREATE_SHLIB+${RELOCATING+. = ${SHLIB_TEXT_START_ADDR:-0} + SIZEOF_HEADERS;}}
   ${CREATE_PIE+${RELOCATING+. = ${SHLIB_TEXT_START_ADDR:-0} + SIZEOF_HEADERS;}}
+  ${CREATE_SHLIB-${RISCOS_IMAGE_HDR}}
+  ${CREATE_SHLIB-${RISCOS_ROBASE}}
   ${CREATE_SHLIB-${INTERP}}
   ${INITIAL_READONLY_SECTIONS}
   ${TEXT_DYNAMIC+${DYNAMIC}}
@@ -362,6 +379,8 @@
   .eh_frame     ${RELOCATING-0} : ONLY_IF_RO { KEEP (*(.eh_frame)) }
   .gcc_except_table ${RELOCATING-0} : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) }
 
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_ROLIMIT}}}
+
   /* Adjust the address for the data segment.  We want to adjust up to
      the same address within the page on the next page up.  */
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+. = ${DATA_ADDR-${DATA_SEGMENT_ALIGN}};}}}
@@ -403,6 +422,7 @@
   ${RELOCATING+${DATARELRO}}
   ${OTHER_RELRO_SECTIONS}
   ${TEXT_DYNAMIC-${DYNAMIC}}
+  ${DATA_GOT+${RISCOS_SOM_GOT_SYMBOL}}
   ${DATA_GOT+${RELRO_NOW+${GOT}}}
   ${DATA_GOT+${RELRO_NOW+${GOTPLT}}}
   ${DATA_GOT+${RELRO_NOW-${SEPARATE_GOTPLT+${GOT}}}}
@@ -412,6 +432,7 @@
 
   ${DATA_PLT+${PLT_BEFORE_GOT-${PLT}}}
 
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWBASE}}}
   .data         ${RELOCATING-0} :
   {
     ${RELOCATING+${DATA_START_SYMBOLS}}
@@ -435,10 +456,15 @@
   ${RELOCATING+${OTHER_BSS_SYMBOLS}}
   ${SBSS}
   ${BSS_PLT+${PLT}}
+  /* RISC OS module __RelocCode & __RelocData */
+  .riscos.module.relocdata : { *(.riscos.module.relocdata) }
+  .riscos.module.reloccode : { *(.riscos.module.reloccode) }
   .bss          ${RELOCATING-0} :
   {
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZIBASE}}}
    *(.dynbss)
-   *(.bss${RELOCATING+ .bss.* .gnu.linkonce.b.*})
+   /* Sort the .bss sections to avoid runtime check errors in SCL.  */
+   *(.bss${RELOCATING+ SORT(.bss.*) .gnu.linkonce.b.*})
    *(COMMON)
    /* Align here to ensure that the .bss section occupies space up to
       _end.  Align after .bss to ensure correct alignment even if the
@@ -446,6 +472,7 @@
       FIXME: Why do we need it? When there is no .bss section, we don't
       pad the .data section.  */
    ${RELOCATING+. = ALIGN(. != 0 ? ${ALIGNMENT} : 1);}
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZILIMIT}}}
   }
   ${RELOCATING+${OTHER_BSS_END_SYMBOLS}}
   ${RELOCATING+. = ALIGN(${ALIGNMENT});}
@@ -454,6 +481,7 @@
   ${RELOCATING+${OTHER_END_SYMBOLS}}
   ${RELOCATING+${END_SYMBOLS-_end = .; PROVIDE (end = .);}}
   ${RELOCATING+${DATA_SEGMENT_END}}
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWLIMIT}}}
 
   /* Stabs debugging sections.  */
   .stab          0 : { *(.stab) }
