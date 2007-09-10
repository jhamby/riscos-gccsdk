--- Makefile.orig	2007-09-05 15:47:27.966607300 +0100
+++ Makefile	2007-09-05 15:48:33.355607300 +0100
@@ -17,7 +17,7 @@

 SDLVERSION=$(shell sdl-config --version 2>/dev/null)
 ROOTDIR=$(shell pwd)
-TARGET=$(shell uname -s | tr [a-z] [A-Z])
+TARGET=$(shell $(GCCSDK_INSTALL_ENV)/uname -s | tr [a-z] [A-Z])

 #
 # Config
@@ -71,7 +71,7 @@
 all:
 	@echo "ROOTDIR=$(ROOTDIR)" > Makefile.global
 	@echo "XOBJ=$(XOBJ)" >> Makefile.global
-	@echo "CFLAGS=-g -ansi -pedantic -Wall -W -O2 -I $(ROOTDIR)/include $(shell sdl-config --cflags)" >> Makefile.global
+	@echo "CFLAGS=-g -ansi -pedantic -Wall -W -O2 -I $(ROOTDIR)/include $(shell sdl-config --cflags) -I$(GCCSDK_INSTALL_ENV)/include" >> Makefile.global
 	@echo "LDFLAGS=-lz $(shell sdl-config --libs)" >> Makefile.global
 	@echo "CC=gcc" >> Makefile.global
 	@echo "CPP=gcc -E" >> Makefile.global
