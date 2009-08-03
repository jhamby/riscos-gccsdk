--- Makefile.orig	2009-08-03 07:22:56.000000000 -0700
+++ Makefile	2009-08-03 07:24:25.000000000 -0700
@@ -17,7 +17,7 @@
 
 SDLVERSION=$(shell sdl-config --version 2>/dev/null)
 ROOTDIR=$(shell pwd)
-TARGET=$(shell uname -s | tr [a-z] [A-Z])
+TARGET=$(shell $(GCCSDK_INSTALL_ENV)/uname -s | tr [a-z] [A-Z])
 
 #
 # Config
@@ -71,10 +71,8 @@
 all:
 	@echo "ROOTDIR=$(ROOTDIR)" > Makefile.global
 	@echo "XOBJ=$(XOBJ)" >> Makefile.global
-	@echo "CFLAGS=-g -ansi -pedantic -Wall -W -O2 -I $(ROOTDIR)/include $(shell sdl-config --cflags)" >> Makefile.global
-	@echo "LDFLAGS=-lz $(shell sdl-config --libs)" >> Makefile.global
-	@echo "CC=gcc" >> Makefile.global
-	@echo "CPP=gcc -E" >> Makefile.global
+	@echo "CFLAGS=-g -ansi -pedantic -Wall -W -O2 -I $(ROOTDIR)/include $(shell sdl-config --cflags) -I$(GCCSDK_INSTALL_ENV)/include" >> Makefile.global
+	@echo "LDFLAGS=-lz $(shell sdl-config --libs) -static" >> Makefile.global
 	$(MAKE) -C src all
 
 clean:
