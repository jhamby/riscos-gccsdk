--- ld/scripttempl/elf.sc.orig	2018-01-13 13:31:16.000000000 +0000
+++ ld/scripttempl/elf.sc	2019-01-14 22:58:26.997491083 +0000
@@ -346,6 +346,14 @@
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
 /* Copyright (C) 2014-2018 Free Software Foundation, Inc.
 
@@ -372,6 +380,7 @@
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+PROVIDE (__executable_start = ${TEXT_START_ADDR}); . = ${TEXT_BASE_ADDRESS};}}}
   ${CREATE_SHLIB+${RELOCATING+. = ${SHLIB_TEXT_START_ADDR}${SIZEOF_HEADERS_CODE};}}
   ${CREATE_PIE+${RELOCATING+PROVIDE (__executable_start = ${SHLIB_TEXT_START_ADDR}); . = ${SHLIB_TEXT_START_ADDR}${SIZEOF_HEADERS_CODE};}}
+  ${CREATE_SHLIB-${RISCOS_ROBASE}}
 EOF
 
 emit_early_ro()
@@ -497,6 +506,8 @@
 cat <<EOF
   ${RELOCATING+${TEXT_SEGMENT_ALIGN}}
 
+  /* RISC OS module header: */
+  .riscos.module.header : { *(.riscos.module.header) }
   .init         ${RELOCATING-0}${RELOCATING+${INIT_ADDR}} :
   {
     ${RELOCATING+${INIT_START}}
@@ -574,6 +585,11 @@
   .exception_ranges*) }
   ${TEXT_PLT+${PLT_NEXT_DATA+${PLT} ${OTHER_PLT_SECTIONS}}}
 
+  /* RISC OS PIC relocs */
+  .riscos.pic : { *(.riscos.pic) }
+
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_ROLIMIT}}}
+
   /* Adjust the address for the data segment.  We want to adjust up to
      the same address within the page on the next page up.  */
   ${CREATE_SHLIB-${CREATE_PIE-${RELOCATING+. = ${DATA_ADDR-${DATA_SEGMENT_ALIGN}};}}}
@@ -616,6 +632,11 @@
 
   ${DATA_PLT+${PLT_BEFORE_GOT-${PLT}}}
 
+  /* RISC OS module __RelocCode & __RelocData */
+  .riscos.module.relocdata : { *(.riscos.module.relocdata) }
+  .riscos.module.reloccode : { *(.riscos.module.reloccode) }
+
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWBASE}}}
   .data         ${RELOCATING-0} :
   {
     ${RELOCATING+${DATA_START_SYMBOLS}}
@@ -641,6 +662,7 @@
   ${BSS_PLT+${PLT}}
   .${BSS_NAME}          ${RELOCATING-0} :
   {
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZIBASE}}}
    ${RELOCATING+*(.dynbss)}
    *(.${BSS_NAME}${RELOCATING+ .${BSS_NAME}.* .gnu.linkonce.b.*})
    *(COMMON)
@@ -650,6 +672,7 @@
       FIXME: Why do we need it? When there is no .bss section, we don't
       pad the .data section.  */
    ${RELOCATING+. = ALIGN(. != 0 ? ${ALIGNMENT} : 1);}
+   ${RELOCATING+${CREATE_SHLIB-${RISCOS_ZILIMIT}}}
   }
   ${OTHER_BSS_SECTIONS}
   ${LARGE_BSS_AFTER_BSS+${LARGE_BSS}}
@@ -673,6 +696,7 @@
   ${TINY_DATA_SECTION}
   ${TINY_BSS_SECTION}
   ${STACK_ADDR+${STACK}}
+  ${RELOCATING+${CREATE_SHLIB-${RISCOS_RWLIMIT}}}
 EOF
 
 test -z "${NON_ALLOC_DYN}" || emit_dyn
