ARCH=arm
SCRIPT_NAME=elf
OUTPUT_FORMAT="elf32-littlearm"
BIG_OUTPUT_FORMAT="elf32-bigarm"
LITTLE_OUTPUT_FORMAT="elf32-littlearm"
# On RISC OS it doesn't have any advantage to have page alignment so we
# disable it by defining it to align on 4 bytes.
MAXPAGESIZE=0x4
COMMONPAGESIZE=0x4
TEMPLATE_NAME=elf32
EXTRA_EM_FILE=armelf
GENERATE_SHLIB_SCRIPT=yes

TEXT_START_SYMBOLS='__text_start = . ;';
DATA_START_SYMBOLS='__data_start = . ;';
OTHER_TEXT_SECTIONS='*(.glue_7t) *(.glue_7) *(.riscos.libscl.chunkstub.start) *(SORT(.riscos.libscl.chunkstub.id*)) *(.riscos.libscl.chunkstub.end)'
OTHER_BSS_SYMBOLS='__bss_start__ = .;'
OTHER_BSS_END_SYMBOLS='_bss_end__ = . ; __bss_end__ = . ; __end__ = . ;'
OTHER_SECTIONS='.note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }'
OTHER_READONLY_SECTIONS="
  .ARM.extab ${RELOCATING-0} : { *(.ARM.extab${RELOCATING+* .gnu.linkonce.armextab.*}) }
  ${RELOCATING+ PROVIDE_HIDDEN (__exidx_start = .); }
  .ARM.exidx ${RELOCATING-0} : { *(.ARM.exidx${RELOCATING+* .gnu.linkonce.armexidx.*}) }
  ${RELOCATING+ PROVIDE_HIDDEN (__exidx_end = .); }"

TEXT_START_ADDR=0x00008000
TARGET2_TYPE=got-rel

# ARM does not support .s* sections.
NO_SMALL_DATA=yes
