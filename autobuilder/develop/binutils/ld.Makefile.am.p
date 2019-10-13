--- ld/Makefile.am.orig	2018-01-13 13:31:16.000000000 +0000
+++ ld/Makefile.am	2018-06-09 21:53:33.184251536 +0100
@@ -179,6 +179,7 @@
 	earmelf_nacl.c \
 	earmelf_nbsd.c \
 	earmelf_phoenix.c \
+	earmelf_riscos_eabi.c \
 	earmelf_vxworks.c \
 	earmelfb.c \
 	earmelfb_fbsd.c \
@@ -795,6 +796,10 @@
   $(ELF_DEPS) $(srcdir)/emultempl/armelf.em \
   $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}
 
+earmelf_riscos_eabi.c: $(srcdir)/emulparams/armelf_riscos_eabi.sh \
+  $(srcdir)/emultempl/elf32.em $(srcdir)/emultempl/armelf.em \
+  $(srcdir)/scripttempl/elf.sc ${GEN_DEPENDS}
+
 earmelf_vxworks.c: $(srcdir)/emulparams/armelf_vxworks.sh \
   $(srcdir)/emulparams/vxworks.sh $(srcdir)/emulparams/armelf.sh \
   $(ELF_DEPS) $(srcdir)/emultempl/vxworks.em \
