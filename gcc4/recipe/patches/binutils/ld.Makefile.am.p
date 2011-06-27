--- ld/Makefile.am.orig	2011-03-16 09:37:30.000000000 +0100
+++ ld/Makefile.am	2011-06-27 23:19:21.141934725 +0200
@@ -144,6 +144,7 @@ ALL_EMULATION_SOURCES = \
 	earmelf_linux.c \
 	earmelf_linux_eabi.c \
 	earmelf_nbsd.c \
+	earmelf_riscos.c \
 	earmelf_vxworks.c \
 	earmelfb.c \
 	earmelfb_linux.c \
@@ -670,6 +671,10 @@ earmelf_fbsd.c: $(srcdir)/emulparams/arm
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
