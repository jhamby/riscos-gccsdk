--- Makefile.orig	2009-07-26 11:35:02.000000000 -0700
+++ Makefile	2009-07-26 11:34:36.000000000 -0700
@@ -22,7 +22,7 @@
   AR := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*ar)
   CFLAGS += -Driscos -mpoke-function-name -I$(GCCSDK_INSTALL_ENV)/include \
 	-I$(GCCSDK_INSTALL_ENV)/include/libxml2
-  LIBS = -L$(GCCSDK_INSTALL_ENV)/lib -lxml2 -lz
+  LIBS = -static -L$(GCCSDK_INSTALL_ENV)/lib -lxml2 -lz
   ifneq (,$(findstring arm-unknown-riscos-gcc,$(CC)))
     EXEEXT := ,e1f
     SUBTARGET := -elf-
