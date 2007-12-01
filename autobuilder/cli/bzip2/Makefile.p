--- Makefile.orig	2007-11-30 20:16:47.000000000 -0800
+++ Makefile	2007-11-30 20:33:16.000000000 -0800
@@ -3,10 +3,10 @@
 SHELL=/bin/sh
 
 # To assist in cross-compiling
-CC=gcc
-AR=ar
+#CC=gcc
+#AR=ar
 RANLIB=ranlib
-LDFLAGS=
+#LDFLAGS=
 
 BIGFILES=-D_FILE_OFFSET_BITS=64
 CFLAGS=-Wall -Winline -O2 -g $(BIGFILES) $(DEBCFLAGS)
@@ -23,13 +23,13 @@
       decompress.o \
       bzlib.o
 
-all: libbz2.a bzip2 bzip2recover # test
+all: libbz2.a bzip2$(AB_EXEEXT) bzip2recover$(AB_EXEEXT) # test
 
-bzip2: libbz2.so bzip2.o
-	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2 bzip2.o -L. -lbz2
+bzip2$(AB_EXEEXT): libbz2.so bzip2.o
+	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2$(AB_EXEEXT) bzip2.o -L. -lbz2
 
-bzip2recover: bzip2recover.o
-	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2recover bzip2recover.o
+bzip2recover$(AB_EXEEXT): bzip2recover.o
+	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2recover$(AB_EXEEXT) bzip2recover.o
 
 libbz2.a: $(OBJS)
 	rm -f libbz2.a
