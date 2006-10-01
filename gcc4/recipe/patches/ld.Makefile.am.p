--- ld/Makefile.am.orig	2006-09-25 03:09:46.000000000 +0200
+++ ld/Makefile.am	2006-09-25 03:09:46.000000000 +0200
@@ -117,6 +117,7 @@
 	earmelf.o \
 	earmelfb.o \
 	earmelf_fbsd.o \
+	earmelf_riscos.o \
 	earmelf_linux.o \
 	earmelf_linux_eabi.o \
 	earmelfb_linux.o \
@@ -511,6 +512,10 @@
   $(srcdir)/emultempl/elf32.em $(srcdir)/emultempl/armelf.em \
   $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}
 	${GENSCRIPTS} armelf_fbsd "$(tdir_armelf_fbsd)"
+earmelf_riscos.c: $(srcdir)/emulparams/armelf_riscos.sh \
+  $(srcdir)/emultempl/elf32.em $(srcdir)/emultempl/armelf.em \
+  $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}
+	${GENSCRIPTS} armelf_riscos "$(tdir_armelf_riscos)"
 earmelf_linux.c: $(srcdir)/emulparams/armelf_linux.sh \
   $(srcdir)/emultempl/elf32.em $(srcdir)/emultempl/armelf.em \
   $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}
