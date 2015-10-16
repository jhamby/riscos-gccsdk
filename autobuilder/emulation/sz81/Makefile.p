--- Makefile.orig	2015-10-16 22:16:27.279034703 +0100
+++ Makefile	2015-10-16 22:17:03.775033893 +0100
@@ -12,7 +12,7 @@
 #PACKAGE_DATA_DIR?=$(PREFIX)/data
 
 # Run from current folder i.e. no installation
-PREFIX?=.
+PREFIX?=/\<Sz81\$$Dir\>
 BINDIR?=$(PREFIX)
 DOCDIR?=$(PREFIX)
 PACKAGE_DATA_DIR?=$(PREFIX)/data
@@ -36,7 +36,7 @@
 CFLAGS?=-O3
 CFLAGS+=-Wall `$(SDL_CONFIG) --cflags` -DVERSION=\"$(VERSION)\" \
 	-DPACKAGE_DATA_DIR=\"$(PACKAGE_DATA_DIR)\" $(SOUNDDEF) -DSZ81 
-LINK=$(CC)
+LINK=$(CC) -static
 LDFLAGS=
 LIBS=`$(SDL_CONFIG) --libs` 
 
