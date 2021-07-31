--- Makefile.orig	2010-01-15 11:41:38.000000000 -0800
+++ Makefile	2010-01-15 11:41:48.000000000 -0800
@@ -72,7 +72,7 @@
 	@echo "ROOTDIR=$(ROOTDIR)" > Makefile.global
 	@echo "XOBJ=$(XOBJ)" >> Makefile.global
 	@echo "CFLAGS=-g -ansi -pedantic -Wall -W -O2 -I $(ROOTDIR)/include $(shell sdl-config --cflags)" >> Makefile.global
-	@echo "LDFLAGS=-lz $(shell sdl-config --libs)" >> Makefile.global
+	@echo "LDFLAGS=-lz $(shell sdl-config --libs) -static" >> Makefile.global
 	@echo "CC=gcc" >> Makefile.global
 	@echo "CPP=gcc -E" >> Makefile.global
 	$(MAKE) -C src all
