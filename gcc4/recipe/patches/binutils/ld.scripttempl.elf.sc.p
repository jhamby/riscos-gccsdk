--- ld/scripttempl/elf.sc.orig	2010-01-14 11:48:48.000000000 +0100
+++ ld/scripttempl/elf.sc	2010-06-29 23:02:21.920877837 +0200
@@ -268,6 +268,14 @@ else
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
@@ -288,6 +296,7 @@ SECTIONS
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+PROVIDE (__executable_start = ${TEXT_START_ADDR}); . = ${TEXT_BASE_ADDRESS};}}}
   ${CREATE_SHLIB+${RELOCATING+. = ${SHLIB_TEXT_START_ADDR} + SIZEOF_HEADERS;}}
   ${CREATE_PIE+${RELOCATING+. = ${SHLIB_TEXT_START_ADDR} + SIZEOF_HEADERS;}}
+  ${CREATE_SHLIB-${RISCOS_ROBASE}}
   ${INITIAL_READONLY_SECTIONS}
   .note.gnu.build-id : { *(.note.gnu.build-id) }
 EOF
@@ -399,6 +408,9 @@ if test -z "${NON_ALLOC_DYN}"; then
 fi
 
 cat <<EOF
+  /* RISC OS module header: */
+  .riscos.module.header : { *(.riscos.module.header) }
+
   .init         ${RELOCATING-0} : 
   { 
     ${RELOCATING+${INIT_START}}
@@ -435,6 +447,8 @@ cat <<EOF
   .eh_frame     ${RELOCATING-0} : ONLY_IF_RO { KEEP (*(.eh_frame)) }
   .gcc_except_table ${RELOCATING-0} : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) }
 
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_ROLIMIT}}}
+
   /* Adjust the address for the data segment.  We want to adjust up to
      the same address within the page on the next page up.  */
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+. = ${DATA_ADDR-${DATA_SEGMENT_ALIGN}};}}}
@@ -485,6 +499,11 @@ cat <<EOF
 
   ${DATA_PLT+${PLT_BEFORE_GOT-${PLT}}}
 
+  /* RISC OS module __RelocCode & __RelocData */
+  .riscos.module.relocdata : { *(.riscos.module.relocdata) }
+  .riscos.module.reloccode : { *(.riscos.module.reloccode) }
+
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWBASE}}}
   .data         ${RELOCATING-0} :
   {
     ${RELOCATING+${DATA_START_SYMBOLS}}
@@ -509,8 +528,10 @@ cat <<EOF
   ${BSS_PLT+${PLT}}
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
@@ -518,6 +539,7 @@ cat <<EOF
       FIXME: Why do we need it? When there is no .bss section, we don't
       pad the .data section.  */
    ${RELOCATING+. = ALIGN(. != 0 ? ${ALIGNMENT} : 1);}
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZILIMIT}}}
   }
   ${OTHER_BSS_SECTIONS}
   ${RELOCATING+${OTHER_BSS_END_SYMBOLS}}
@@ -527,6 +549,7 @@ cat <<EOF
   ${RELOCATING+${OTHER_END_SYMBOLS}}
   ${RELOCATING+${END_SYMBOLS-${USER_LABEL_PREFIX}_end = .; PROVIDE (${USER_LABEL_PREFIX}end = .);}}
   ${RELOCATING+${DATA_SEGMENT_END}}
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWLIMIT}}}
 EOF
 
 if test -n "${NON_ALLOC_DYN}"; then
