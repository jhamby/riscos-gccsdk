--- Makefile.orig	2008-01-12 13:34:14.000000000 -0800
+++ Makefile	2008-01-12 13:34:29.000000000 -0800
@@ -17,16 +17,16 @@
 SHELL=/bin/sh
 
 # To assist in cross-compiling
-CC=gcc
-AR=ar
+#CC=gcc
+#AR=ar
 RANLIB=ranlib
-LDFLAGS=
+LDFLAGS=-static
 
 BIGFILES=-D_FILE_OFFSET_BITS=64
 CFLAGS=-Wall -Winline -O2 -g $(BIGFILES) $(DEBCFLAGS)
 
 # Where you want it installed when you do 'make install'
-PREFIX=/usr/local
+PREFIX=$(GCCSDK_INSTALL_ENV)
 
 
 OBJS= blocksort.o  \
@@ -37,13 +37,13 @@
       decompress.o \
       bzlib.o
 
-all: libbz2.a bzip2 bzip2recover # test
+all: libbz2.a bzip2$(AB_EXEEXT) bzip2recover$(AB_EXEEXT) # test
 
-bzip2: libbz2.so bzip2.o
-	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2 bzip2.o -L. -lbz2
+bzip2$(AB_EXEEXT): libbz2.a bzip2.o
+	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2$(AB_EXEEXT) bzip2.o -L. -lbz2
 
-bzip2recover: bzip2recover.o
-	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2recover bzip2recover.o
+bzip2recover$(AB_EXEEXT): bzip2recover.o
+	$(CC) $(CFLAGS) $(LDFLAGS) -o bzip2recover$(AB_EXEEXT) bzip2recover.o
 
 libbz2.a: $(OBJS)
 	rm -f libbz2.a
@@ -93,15 +93,15 @@
 	cmp sample3.tst sample3.ref
 	@cat words3
 
-install: bzip2 bzip2recover libbz2.a
+install: bzip2$(AB_EXEEXT) bzip2recover$(AB_EXEEXT) libbz2.a
 	if ( test ! -d $(PREFIX)/bin ) ; then mkdir -p $(PREFIX)/bin ; fi
 	if ( test ! -d $(PREFIX)/lib ) ; then mkdir -p $(PREFIX)/lib ; fi
 	if ( test ! -d $(PREFIX)/man ) ; then mkdir -p $(PREFIX)/man ; fi
 	if ( test ! -d $(PREFIX)/man/man1 ) ; then mkdir -p $(PREFIX)/man/man1 ; fi
 	if ( test ! -d $(PREFIX)/include ) ; then mkdir -p $(PREFIX)/include ; fi
 	cp -f bzip2 $(PREFIX)/bin/bzip2
-	ln $(PREFIX)/bin/bzip2 $(PREFIX)/bin/bunzip2
-	ln $(PREFIX)/bin/bzip2 $(PREFIX)/bin/bzcat
+	#ln $(PREFIX)/bin/bzip2 $(PREFIX)/bin/bunzip2
+	#ln $(PREFIX)/bin/bzip2 $(PREFIX)/bin/bzcat
 	cp -f bzip2recover $(PREFIX)/bin/bzip2recover
 	chmod a+x $(PREFIX)/bin/bzip2
 	chmod a+x $(PREFIX)/bin/bunzip2
@@ -111,7 +111,7 @@
 	chmod a+r $(PREFIX)/man/man1/bzip2.1
 	cp -f bzlib.h $(PREFIX)/include
 	chmod a+r $(PREFIX)/include/bzlib.h
-	cp -fa libbz2.a libbz2.so* $(PREFIX)/lib
+	cp -fa libbz2.a $(PREFIX)/lib
 	chmod a+r $(PREFIX)/lib/libbz2.a
 	cp -f bzexe $(PREFIX)/bin/bzexe
 	chmod a+x $(PREFIX)/bin/bzexe
