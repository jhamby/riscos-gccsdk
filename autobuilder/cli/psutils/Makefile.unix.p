--- Makefile.unix.orig	2021-01-18 16:31:09.530992768 +0000
+++ Makefile.unix	2021-01-18 16:31:32.454538833 +0000
@@ -28,7 +28,7 @@
 BINDIR = /usr/local/bin
 SCRIPTDIR = $(BINDIR)
 INCLUDEDIR = /usr/local/share/psutils
-PERL = /usr/local/bin/perl
+PERL = perl
 
 BINMODE = 0755
 MANMODE = 0644
@@ -39,17 +39,17 @@
 MANDIR = /usr/local/share/man/man$(MANEXT)
 
 CC = gcc
-CFLAGS = -DPAPER=\"$(PAPER)\" -DUNIX -O -Wall
+CFLAGS = -DPAPER=\"$(PAPER)\" -DUNIX -O3 -Wall
 
 BIN = psbook psselect pstops epsffit psnup \
 	psresize
 SHELLSCRIPTS = getafm showchar
-PERLSCRIPTS = fixfmps fixmacps fixpsditps fixpspps \
+PERLSCRIPTS = fixfmps fixpsditps fixpspps \
 	fixtpps fixwfwps fixwpps fixscribeps fixwwps \
 	fixdlsrps extractres includeres psmerge
 MANPAGES = psbook.$(MANEXT) psselect.$(MANEXT) pstops.$(MANEXT) epsffit.$(MANEXT) psnup.$(MANEXT) \
 	psresize.$(MANEXT) psmerge.$(MANEXT) fixscribeps.$(MANEXT) getafm.$(MANEXT) \
-	fixdlsrps.$(MANEXT) fixfmps.$(MANEXT) fixmacps.$(MANEXT) fixpsditps.$(MANEXT) \
+	fixdlsrps.$(MANEXT) fixfmps.$(MANEXT) fixpsditps.$(MANEXT) \
 	fixpspps.$(MANEXT) fixtpps.$(MANEXT) fixwfwps.$(MANEXT) fixwpps.$(MANEXT) \
 	fixwwps.$(MANEXT) extractres.$(MANEXT) includeres.$(MANEXT)
 INCLUDES = md68_0.ps md71_0.ps
@@ -65,30 +65,30 @@
 epsffit.o: epsffit.c pserror.h patchlev.h
 
 epsffit: epsffit.o pserror.o
-	$(CC) $(CCFLAGS) -o epsffit pserror.o epsffit.o
+	$(CC) $(CCFLAGS) -static -o epsffit$(AB_EXEEXT) pserror.o epsffit.o
 
 psnup: psnup.o psutil.o psspec.o pserror.o
-	$(CC) $(CCFLAGS) -o psnup psutil.o psspec.o pserror.o psnup.o
+	$(CC) $(CCFLAGS) -static -o psnup$(AB_EXEEXT) psutil.o psspec.o pserror.o psnup.o
 
 psnup.o: psutil.h patchlev.h psspec.h pserror.h psnup.c
 
 psresize: psresize.o psutil.o pserror.o psspec.o
-	$(CC) $(CCFLAGS) -o psresize psutil.o psspec.o pserror.o psresize.o
+	$(CC) $(CCFLAGS) -static -o psresize$(AB_EXEEXT) psutil.o psspec.o pserror.o psresize.o
 
 psresize.o: psutil.h patchlev.h psspec.h pserror.h psresize.c
 
 psbook: psbook.o psutil.o pserror.o
-	$(CC) $(CCFLAGS) -o psbook psutil.o pserror.o psbook.o
+	$(CC) $(CCFLAGS) -static -o psbook$(AB_EXEEXT) psutil.o pserror.o psbook.o
 
 psbook.o: psutil.h patchlev.h pserror.h psbook.c
 
 psselect: psselect.o psutil.o pserror.o
-	$(CC) $(CCFLAGS) -o psselect psutil.o pserror.o psselect.o
+	$(CC) $(CCFLAGS) -static -o psselect$(AB_EXEEXT) psutil.o pserror.o psselect.o
 
 psselect.o: psutil.h patchlev.h pserror.h psselect.c
 
 pstops: pstops.o psutil.o psspec.o pserror.o
-	$(CC) $(CCFLAGS) -o pstops psutil.o psspec.o pserror.o pstops.o
+	$(CC) $(CCFLAGS) -static -o pstops$(AB_EXEEXT) psutil.o psspec.o pserror.o pstops.o
 
 pstops.o: psutil.h patchlev.h psspec.h pserror.h pstops.c
 
