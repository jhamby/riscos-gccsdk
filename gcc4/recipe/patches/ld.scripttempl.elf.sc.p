--- ld/scripttempl/elf.sc.orig	2007-05-19 15:17:58.000000000 +0100
+++ ld/scripttempl/elf.sc	2007-05-27 19:18:00.000000000 +0100
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
@@ -358,16 +375,24 @@
   ${CREATE_SHLIB-${SDATA2}}
   ${CREATE_SHLIB-${SBSS2}}
   ${OTHER_READONLY_SECTIONS}
+
   .eh_frame_hdr : { *(.eh_frame_hdr) }
   .eh_frame     ${RELOCATING-0} : ONLY_IF_RO { KEEP (*(.eh_frame)) }
   .gcc_except_table ${RELOCATING-0} : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) }
 
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_ROLIMIT}}}
+
   /* Adjust the address for the data segment.  We want to adjust up to
      the same address within the page on the next page up.  */
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+. = ${DATA_ADDR-${DATA_SEGMENT_ALIGN}};}}}
   ${CREATE_SHLIB+${RELOCATING+. = ${SHLIB_DATA_ADDR-${DATA_SEGMENT_ALIGN}};}}
   ${CREATE_PIE+${RELOCATING+. = ${SHLIB_DATA_ADDR-${DATA_SEGMENT_ALIGN}};}}
 
+  /* RISC OS module __RelocCode & __RelocData */
+  .riscos.module.reloccode : { *(.riscos.module.reloccode) }
+  .riscos.module.relocdata : { *(.riscos.module.relocdata) }
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWBASE}}}
+
   /* Exception handling  */
   .eh_frame     ${RELOCATING-0} : ONLY_IF_RW { KEEP (*(.eh_frame)) }
   .gcc_except_table ${RELOCATING-0} : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) }
@@ -403,6 +428,7 @@
   ${RELOCATING+${DATARELRO}}
   ${OTHER_RELRO_SECTIONS}
   ${TEXT_DYNAMIC-${DYNAMIC}}
+  ${DATA_GOT+${RISCOS_SOM_GOT_SYMBOL}}
   ${DATA_GOT+${RELRO_NOW+${GOT}}}
   ${DATA_GOT+${RELRO_NOW+${GOTPLT}}}
   ${DATA_GOT+${RELRO_NOW-${SEPARATE_GOTPLT+${GOT}}}}
@@ -437,6 +463,7 @@
   ${BSS_PLT+${PLT}}
   .bss          ${RELOCATING-0} :
   {
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZIBASE}}}
    *(.dynbss)
    *(.bss${RELOCATING+ .bss.* .gnu.linkonce.b.*})
    *(COMMON)
@@ -446,6 +473,7 @@
       FIXME: Why do we need it? When there is no .bss section, we don't
       pad the .data section.  */
    ${RELOCATING+. = ALIGN(. != 0 ? ${ALIGNMENT} : 1);}
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZILIMIT}}}
   }
   ${RELOCATING+${OTHER_BSS_END_SYMBOLS}}
   ${RELOCATING+. = ALIGN(${ALIGNMENT});}
@@ -454,6 +482,7 @@
   ${RELOCATING+${OTHER_END_SYMBOLS}}
   ${RELOCATING+${END_SYMBOLS-_end = .; PROVIDE (end = .);}}
   ${RELOCATING+${DATA_SEGMENT_END}}
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWLIMIT}}}
 
   /* Stabs debugging sections.  */
   .stab          0 : { *(.stab) }
