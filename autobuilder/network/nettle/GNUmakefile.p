Index: GNUmakefile
===================================================================
RCS file: /cvsroot/nettle/nettle/GNUmakefile,v
retrieving revision 1.3
diff -u -r1.3 GNUmakefile
--- GNUmakefile	2 Jun 2012 17:20:34 -0000	1.3
+++ GNUmakefile	3 Jun 2012 12:24:11 -0000
@@ -2,12 +2,12 @@
 # Makefile for cross-compiling Nettle
 #

-CC = /home/riscos/gccsdk/cross/bin/arm-unknown-riscos-gcc
+CC = gcc
 NATIVE_CC = /usr/bin/gcc
-CFLAGS = -c -D_BSD_SOURCE -std=c9x -Wall -mpoke-function-name -I/home/riscos/gccsdk/env/include
+CFLAGS = -c -D_BSD_SOURCE -std=c9x -Wall -mpoke-function-name
 CONFIG = -DWITH_SSH

-LIBS = -L/home/riscos/gccsdk/env/lib -lssh2 -lgcrypt -lgpg-error -lcrypto
+LIBS = -lssh2 -lz -lgcrypt -lgpg-error -lcrypto
 OBJS = chardefn.o choices.o dnslib.o globals.o init.o keyboard.o lineedit.o \
        main.o misc.o hotlist.o nettle.o process.o socket.o quit.o seln.o \
        wimp.o zapgen.o fortify.o sockwatch.o messages.o termlist.o url.o \
@@ -24,7 +24,7 @@

 !Nettle/!RunImage,ff8: !Nettle/!RunImage,e1f
 	@echo " ELF2AIF:" $@
-	@/home/riscos/gccsdk/cross/bin/elf2aif $< $@
+	@elf2aif $< $@
 	@rm !Nettle/!RunImage,e1f

 !Nettle/!RunImage,e1f: $(OBJS)
