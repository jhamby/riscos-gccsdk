--- GNUmakefile.orig	2021-08-05 15:57:56.404267567 -0700
+++ GNUmakefile	2021-08-05 15:58:26.756462207 -0700
@@ -2,12 +2,12 @@
 # Makefile for cross-compiling Nettle
 #
 
-CC = /home/riscos/gccsdk/cross/bin/arm-unknown-riscos-gcc
+#CC = arm-riscos-gnueabihf-gcc
 NATIVE_CC = /usr/bin/gcc
-CFLAGS = -c -D_BSD_SOURCE -std=c9x -Wall -mpoke-function-name -I/home/riscos/gccsdk/env/include
+CFLAGS = -c -D_BSD_SOURCE -std=c9x -Wall
 CONFIG = -DWITH_SSH
 
-LIBS = -L/home/riscos/gccsdk/env/lib -lssh2 -lgcrypt -lgpg-error -lcrypto
+LIBS = -lssh2 -lz -lgcrypt -lgpg-error -lcrypto -ldl
 OBJS = chardefn.o choices.o dnslib.o globals.o init.o keyboard.o lineedit.o \
        main.o misc.o hotlist.o nettle.o process.o socket.o quit.o seln.o \
        wimp.o zapgen.o fortify.o sockwatch.o messages.o termlist.o url.o \
@@ -20,12 +20,13 @@
 	  wimputil.h zapredraw.h
 TEMPLATES = !Nettle/Resources/UK/Templates,fec
 
-all:	!Nettle/!RunImage,ff8
+#all:	!Nettle/!RunImage,ff8
+all:	!Nettle/!RunImage,e1f
 
-!Nettle/!RunImage,ff8: !Nettle/!RunImage,e1f
-	@echo " ELF2AIF:" $@
-	@/home/riscos/gccsdk/cross/bin/elf2aif $< $@
-	@rm !Nettle/!RunImage,e1f
+#!Nettle/!RunImage,ff8: !Nettle/!RunImage,e1f
+#	@echo " ELF2AIF:" $@
+#	@elf2aif $< $@
+#	@rm !Nettle/!RunImage,e1f
 
 !Nettle/!RunImage,e1f: $(OBJS)
 	@echo "    LINK:" $@
