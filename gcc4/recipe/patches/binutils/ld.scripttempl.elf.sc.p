--- ld/scripttempl/elf.sc.orig	2012-09-04 14:53:47.000000000 +0200
+++ ld/scripttempl/elf.sc	2012-11-17 15:01:09.085425342 +0100
@@ -316,6 +316,14 @@ else
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
 cat <<EOF
 OUTPUT_FORMAT("${OUTPUT_FORMAT}", "${BIG_OUTPUT_FORMAT}",
 	      "${LITTLE_OUTPUT_FORMAT}")
@@ -336,6 +344,7 @@ SECTIONS
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+PROVIDE (__executable_start = ${TEXT_START_ADDR}); . = ${TEXT_BASE_ADDRESS};}}}
   ${CREATE_SHLIB+${RELOCATING+. = ${SHLIB_TEXT_START_ADDR}${SIZEOF_HEADERS_CODE};}}
   ${CREATE_PIE+${RELOCATING+. = ${SHLIB_TEXT_START_ADDR}${SIZEOF_HEADERS_CODE};}}
+  ${CREATE_SHLIB-${RISCOS_ROBASE}}
 EOF
 
 emit_early_ro()
@@ -459,6 +468,8 @@ emit_dyn()
 test -n "${NON_ALLOC_DYN}${SEPARATE_CODE}" || emit_dyn
 
 cat <<EOF
+  /* RISC OS module header: */
+  .riscos.module.header : { *(.riscos.module.header) }
   .init         ${RELOCATING-0} :
   {
     ${RELOCATING+${INIT_START}}
@@ -517,6 +528,11 @@ cat <<EOF
   .exception_ranges ${RELOCATING-0} : ONLY_IF_RO { *(.exception_ranges
   .exception_ranges*) }
 
+  /* RISC OS PIC relocs */
+  .riscos.pic : { *(.riscos.pic) }
+
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_ROLIMIT}}}
+
   /* Adjust the address for the data segment.  We want to adjust up to
      the same address within the page on the next page up.  */
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+. = ${DATA_ADDR-${DATA_SEGMENT_ALIGN}};}}}
@@ -556,6 +572,11 @@ cat <<EOF
 
   ${DATA_PLT+${PLT_BEFORE_GOT-${PLT}}}
 
+  /* RISC OS module __RelocCode & __RelocData */
+  .riscos.module.relocdata : { *(.riscos.module.relocdata) }
+  .riscos.module.reloccode : { *(.riscos.module.reloccode) }
+
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWBASE}}}
   .data         ${RELOCATING-0} :
   {
     ${RELOCATING+${DATA_START_SYMBOLS}}
@@ -581,8 +602,11 @@ cat <<EOF
   ${BSS_PLT+${PLT}}
   .${BSS_NAME}          ${RELOCATING-0} :
   {
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZIBASE}}}
    *(.dyn${BSS_NAME})
    *(.${BSS_NAME}${RELOCATING+ .${BSS_NAME}.* .gnu.linkonce.b.*})
+   /* Sort the .${BSS_NAME} sections to avoid runtime check errors in SCL.  */
+   *(.${BSS_NAME}${RELOCATING+ SORT(.${BSS_NAME}.*) .gnu.linkonce.b.*})
    *(COMMON)
    /* Align here to ensure that the .bss section occupies space up to
       _end.  Align after .bss to ensure correct alignment even if the
@@ -590,6 +614,7 @@ cat <<EOF
       FIXME: Why do we need it? When there is no .bss section, we don't
       pad the .data section.  */
    ${RELOCATING+. = ALIGN(. != 0 ? ${ALIGNMENT} : 1);}
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZILIMIT}}}
   }
   ${OTHER_BSS_SECTIONS}
   ${RELOCATING+${OTHER_BSS_END_SYMBOLS}}
@@ -599,6 +624,7 @@ cat <<EOF
   ${RELOCATING+${OTHER_END_SYMBOLS}}
   ${RELOCATING+${END_SYMBOLS-${USER_LABEL_PREFIX}_end = .; PROVIDE (${USER_LABEL_PREFIX}end = .);}}
   ${RELOCATING+${DATA_SEGMENT_END}}
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWLIMIT}}}
 EOF
 
 test -z "${NON_ALLOC_DYN}" || emit_dyn
