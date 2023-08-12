--- Makefile.orig	2020-06-13 08:26:09.830314511 +0100
+++ Makefile	2020-06-13 08:28:04.252046239 +0100
@@ -13,20 +13,20 @@
 # ------------------------------------------------------------------
 
 somajor=1.0
-sominor=$(somajor).4
+sominor=$(somajor).6
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
-CFLAGS=-Wall -Winline -O2 -g $(BIGFILES)
+CFLAGS=-Wall -Winline -O3 -g $(BIGFILES)
 
 # Where you want it installed when you do 'make install'
-PREFIX=/usr/local
+PREFIX=$(GCCSDK_INSTALL_ENV)
 
 
 OBJS= blocksort.o  \
@@ -37,13 +37,13 @@
       decompress.o \
       bzlib.o
 
-all: libbz2.a bzip2 bzip2recover test
+all: libbz2.a bzip2$(AB_EXEEXT) bzip2recover$(AB_EXEEXT) libbz2.so # test
 
-bzip2: libbz2.so bzip2.o
-	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o bzip2 bzip2.o -L. -lbz2
+bzip2$(AB_EXEEXT): libbz2.so bzip2.o
+	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o bzip2$(AB_EXEEXT) bzip2.o -L. -lbz2
 
-bzip2recover: bzip2recover.o
-	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o bzip2recover bzip2recover.o
+bzip2recover$(AB_EXEEXT): bzip2recover.o
+	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o bzip2recover$(AB_EXEEXT) bzip2recover.o
 
 libbz2.a: $(OBJS)
 	rm -f libbz2.a
@@ -61,7 +61,7 @@
 	ln -sf $^ $@
 
 libbz2.so.$(sominor): $(OBJS:%.o=%.sho)
-	$(CC) -o libbz2.so.$(sominor) -shared $(LDFLAGS) \
+	$(CC) -o libbz2.so.$(sominor) -shared \
 	  -Wl,-soname,libbz2.so.$(somajor) $^ -lc
 
 $(OBJS:%.o=%.sho) bzip2.sho bzip2recover.sho: %.sho: %.c
@@ -92,44 +92,44 @@
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
-	cp -f bzip2recover $(PREFIX)/bin/bzip2recover
-	chmod a+x $(PREFIX)/bin/bzip2
-	chmod a+x $(PREFIX)/bin/bunzip2
-	chmod a+x $(PREFIX)/bin/bzcat
-	chmod a+x $(PREFIX)/bin/bzip2recover
-	cp -f bzip2.1 $(PREFIX)/man/man1
-	chmod a+r $(PREFIX)/man/man1/bzip2.1
+#	ln $(PREFIX)/bin/bzip2 $(PREFIX)/bin/bunzip2
+#	ln $(PREFIX)/bin/bzip2 $(PREFIX)/bin/bzcat
+#	cp -f bzip2recover $(PREFIX)/bin/bzip2recover
+#	chmod a+x $(PREFIX)/bin/bzip2
+#	chmod a+x $(PREFIX)/bin/bunzip2
+#	chmod a+x $(PREFIX)/bin/bzcat
+#	chmod a+x $(PREFIX)/bin/bzip2recover
+#	cp -f bzip2.1 $(PREFIX)/man/man1
+#	chmod a+r $(PREFIX)/man/man1/bzip2.1
 	cp -f bzlib.h $(PREFIX)/include
 	chmod a+r $(PREFIX)/include/bzlib.h
 	cp -fa libbz2.a libbz2.so* $(PREFIX)/lib
 	chmod a+r $(PREFIX)/lib/libbz2.a
-	cp -f bzgrep $(PREFIX)/bin/bzgrep
-	ln -s -f $(PREFIX)/bin/bzgrep $(PREFIX)/bin/bzegrep
-	ln -s -f $(PREFIX)/bin/bzgrep $(PREFIX)/bin/bzfgrep
-	chmod a+x $(PREFIX)/bin/bzgrep
-	cp -f bzmore $(PREFIX)/bin/bzmore
-	ln -s -f $(PREFIX)/bin/bzmore $(PREFIX)/bin/bzless
-	chmod a+x $(PREFIX)/bin/bzmore
-	cp -f bzdiff $(PREFIX)/bin/bzdiff
-	ln -s -f $(PREFIX)/bin/bzdiff $(PREFIX)/bin/bzcmp
-	chmod a+x $(PREFIX)/bin/bzdiff
-	cp -f bzgrep.1 bzmore.1 bzdiff.1 $(PREFIX)/man/man1
-	chmod a+r $(PREFIX)/man/man1/bzgrep.1
-	chmod a+r $(PREFIX)/man/man1/bzmore.1
-	chmod a+r $(PREFIX)/man/man1/bzdiff.1
-	echo ".so man1/bzgrep.1" > $(PREFIX)/man/man1/bzegrep.1
-	echo ".so man1/bzgrep.1" > $(PREFIX)/man/man1/bzfgrep.1
-	echo ".so man1/bzmore.1" > $(PREFIX)/man/man1/bzless.1
-	echo ".so man1/bzdiff.1" > $(PREFIX)/man/man1/bzcmp.1
+#	cp -f bzgrep $(PREFIX)/bin/bzgrep
+#	ln -s -f $(PREFIX)/bin/bzgrep $(PREFIX)/bin/bzegrep
+#	ln -s -f $(PREFIX)/bin/bzgrep $(PREFIX)/bin/bzfgrep
+#	chmod a+x $(PREFIX)/bin/bzgrep
+#	cp -f bzmore $(PREFIX)/bin/bzmore
+#	ln -s -f $(PREFIX)/bin/bzmore $(PREFIX)/bin/bzless
+#	chmod a+x $(PREFIX)/bin/bzmore
+#	cp -f bzdiff $(PREFIX)/bin/bzdiff
+#	ln -s -f $(PREFIX)/bin/bzdiff $(PREFIX)/bin/bzcmp
+#	chmod a+x $(PREFIX)/bin/bzdiff
+#	cp -f bzgrep.1 bzmore.1 bzdiff.1 $(PREFIX)/man/man1
+#	chmod a+r $(PREFIX)/man/man1/bzgrep.1
+#	chmod a+r $(PREFIX)/man/man1/bzmore.1
+#	chmod a+r $(PREFIX)/man/man1/bzdiff.1
+#	echo ".so man1/bzgrep.1" > $(PREFIX)/man/man1/bzegrep.1
+#	echo ".so man1/bzgrep.1" > $(PREFIX)/man/man1/bzfgrep.1
+#	echo ".so man1/bzmore.1" > $(PREFIX)/man/man1/bzless.1
+#	echo ".so man1/bzdiff.1" > $(PREFIX)/man/man1/bzcmp.1
 
 clean: 
 	rm -f *.o *.sho libbz2.a libbz2.so* bzip2 bzip2recover \
