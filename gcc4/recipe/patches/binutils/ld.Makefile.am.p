--- ld/Makefile.am.orig	2013-11-26 12:37:33.000000000 +0100
+++ ld/Makefile.am	2013-12-31 13:16:36.490126266 +0100
@@ -163,6 +163,7 @@ ALL_EMULATION_SOURCES = \
 	earmelf_linux_eabi.c \
 	earmelf_nacl.c \
 	earmelf_nbsd.c \
+	earmelf_riscos.c \
 	earmelf_vxworks.c \
 	earmelfb.c \
 	earmelfb_linux.c \
@@ -734,6 +735,10 @@ earmelf_fbsd.c: $(srcdir)/emulparams/arm
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
