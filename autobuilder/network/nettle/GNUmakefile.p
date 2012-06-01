Index: GNUmakefile
===================================================================
RCS file: /cvsroot/nettle/nettle/GNUmakefile,v
retrieving revision 1.2
diff -u -r1.2 GNUmakefile
--- GNUmakefile	21 Sep 2010 17:46:13 -0000	1.2
+++ GNUmakefile	1 Jun 2012 09:44:00 -0000
@@ -2,11 +2,11 @@
 # Makefile for cross-compiling Nettle
 #
 
-CC = /home/riscos/cross/bin/arm-unknown-riscos-gcc
-CFLAGS = -c -std=c9x -Wall -mpoke-function-name -I/home/riscos/env/include
+CC = gcc
+CFLAGS = -c -std=c9x -Wall -mpoke-function-name
 CONFIG = -DWITH_SSH
 
-LIBS = -L/home/riscos/env/lib -lssh2 -lgcrypt -lgpg-error
+LIBS = -lssh2 -lgcrypt -lgpg-error -lcrypto
 OBJS = chardefn.o choices.o dnslib.o globals.o init.o keyboard.o lineedit.o \
        main.o misc.o hotlist.o nettle.o process.o socket.o quit.o seln.o \
        wimp.o zapgen.o fortify.o sockwatch.o messages.o termlist.o url.o \
@@ -23,7 +23,7 @@
 
 !Nettle/!RunImage,ff8: RunImage,e1f
 	@echo " ELF2AIF:" $@
-	@/home/riscos/cross/bin/elf2aif $< $@
+	@elf2aif $< $@
 
 RunImage,e1f: $(OBJS)
 	@echo "    LINK:" $@
@@ -37,7 +37,7 @@
 
 # intentionally compiled using native compiler
 templheadr: templheadr.c
-	gcc -o $@ $(LINKFLAGS) $?
+	/usr/bin/gcc -o $@ $(LINKFLAGS) $?
 
 templ.h: templheadr $(TEMPLATES)
 	./templheadr $(TEMPLATES) $@
