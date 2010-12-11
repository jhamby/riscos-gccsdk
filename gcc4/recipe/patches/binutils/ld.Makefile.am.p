--- ld/Makefile.am.orig	2010-11-03 04:22:01.000000000 +0100
+++ ld/Makefile.am	2010-12-11 02:19:04.042493950 +0100
@@ -147,6 +147,7 @@ ALL_EMULATION_SOURCES = \
 	earmelfb_linux_eabi.c \
 	earmelf_nbsd.c \
 	earmelfb_nbsd.c \
+	earmelf_riscos.c \
 	earmelf_vxworks.c \
 	earmnto.c \
 	earmnbsd.c \
@@ -653,6 +654,10 @@ earmelf_fbsd.c: $(srcdir)/emulparams/arm
   $(ELF_DEPS) $(srcdir)/emultempl/armelf.em \
   $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}
 	${GENSCRIPTS} armelf_fbsd "$(tdir_armelf_fbsd)"
+earmelf_riscos.c: $(srcdir)/emulparams/armelf_riscos.sh \
+  $(srcdir)/emultempl/elf32.em $(srcdir)/emultempl/armelf.em \
+  $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}
+	${GENSCRIPTS} armelf_riscos "$(tdir_armelf_riscos)"
 earmelf_linux.c: $(srcdir)/emulparams/armelf_linux.sh \
   $(ELF_DEPS) $(srcdir)/emultempl/armelf.em \
   $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}
