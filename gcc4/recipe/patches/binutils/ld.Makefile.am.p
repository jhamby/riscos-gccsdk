--- ld/Makefile.am.orig	2009-09-01 22:56:51.000000000 +0200
+++ ld/Makefile.am	2010-01-31 16:19:42.245014570 +0100
@@ -127,6 +127,7 @@ ALL_EMULATIONS = \
 	earmelf.o \
 	earmelfb.o \
 	earmelf_fbsd.o \
+	earmelf_riscos.o \
 	earmelf_linux.o \
 	earmelf_linux_eabi.o \
 	earmelfb_linux.o \
@@ -620,6 +621,10 @@ earmelf_fbsd.c: $(srcdir)/emulparams/arm
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
