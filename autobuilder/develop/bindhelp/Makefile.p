--- Tools/BindHelp/Makefile.orig	2014-05-23 12:09:38.000000000 +0100
+++ Tools/BindHelp/Makefile	2014-05-23 10:53:11.000000000 +0100
@@ -3,8 +3,7 @@
 
 .PHONY: all install clean
 OBJS = \
-	BindHelp.o \
-	oslibsupport.o
+	BindHelp.o
 
 # In case they are not defined, use default values.
 CC      ?= /usr/bin/gcc
@@ -17,7 +16,7 @@
 bindir ?= ../../Bin
 
 # No user options from here on:
-ALL_CFLAGS = -Wall -funsigned-char -I.. -I../support $(CFLAGS)
+ALL_CFLAGS = -Wall -funsigned-char $(CFLAGS)
 ALL_LDFLAGS = $(LDFLAGS)
 
 TARGET = bindhelp
