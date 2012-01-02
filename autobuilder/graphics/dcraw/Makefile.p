--- Makefile.orig	2011-12-31 18:07:37.000000000 +0000
+++ Makefile	2011-12-31 18:10:45.000000000 +0000
@@ -1,32 +1,32 @@
 # Executables
-CC     = gcc
+CC     := $(CC) -L$(GCCSDK_INSTALL_ENV)/lib -static
 INSTALL= /usr/bin/install
 
 # Flags
-CFLAGS = -I. -O4 -g -Wall
+CFLAGS = -I. -O4 -g -Wall -I$(GCCSDK_INSTALL_ENV)/include
 
 SRC = dcraw.c parse.c fujiturn.c fuji_green.c clean_crw.c
 OBJ = dcraw.o parse.o fujiturn.o fujiturn16.o fuji_green.o clean_crw.o
 
 # Targets
-default: dcraw dcparse dcfujiturn dcfujiturn16 dcfujigreen dccleancrw
+default: dcraw$(AB_EXEEXT) dcparse$(AB_EXEEXT) dcfujiturn$(AB_EXEEXT) dcfujiturn16$(AB_EXEEXT) dcfujigreen$(AB_EXEEXT) dccleancrw$(AB_EXEEXT)
 
-dcraw: dcraw.o
-	$(CC) -o $@ dcraw.o -lm -ljpeg -llcms
+dcraw$(AB_EXEEXT): dcraw.o
+	$(CC) -o $@ dcraw.o -lm -ljpeg -llcms -ljasper
 
-dcparse: parse.o
+dcparse$(AB_EXEEXT): parse.o
 	$(CC) -o $@ parse.o
 
-dcfujiturn: fujiturn.o
+dcfujiturn$(AB_EXEEXT): fujiturn.o
 	$(CC) -o $@ fujiturn.o
 
-dcfujiturn16: fujiturn16.o
+dcfujiturn16$(AB_EXEEXT): fujiturn16.o
 	$(CC) -o $@ fujiturn16.o
 
-dcfujigreen: fuji_green.o
+dcfujigreen$(AB_EXEEXT): fuji_green.o
 	$(CC) -o $@ fuji_green.o -lm
 
-dccleancrw: clean_crw.o
+dccleancrw$(AB_EXEEXT): clean_crw.o
 	$(CC) -o $@ clean_crw.o
 
 clean:
