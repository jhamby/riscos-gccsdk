--- ld/Makefile.am.orig	2012-09-04 14:53:47.000000000 +0200
+++ ld/Makefile.am	2012-11-17 14:55:35.961613275 +0100
@@ -145,6 +145,7 @@ ALL_EMULATION_SOURCES = \
 	earmelf_linux_eabi.c \
 	earmelf_nacl.c \
 	earmelf_nbsd.c \
+	earmelf_riscos.c \
 	earmelf_vxworks.c \
 	earmelfb.c \
 	earmelfb_linux.c \
@@ -710,6 +711,10 @@ earmelf_fbsd.c: $(srcdir)/emulparams/arm
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
