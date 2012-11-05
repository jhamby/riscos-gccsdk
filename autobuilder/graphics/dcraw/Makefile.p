--- Makefile.orig	2012-11-04 21:23:50.000000000 +0000
+++ Makefile	2012-11-04 21:23:22.000000000 +0000
@@ -0,0 +1,60 @@
+# Executables
+CC     := $(CC) -L$(GCCSDK_INSTALL_ENV)/lib -static
+INSTALL= /usr/bin/install
+
+# Flags
+CFLAGS = -I. -O3 -g -Wall -I$(GCCSDK_INSTALL_ENV)/include
+
+SRC = dcraw.c parse.c fujiturn.c fuji_green.c clean_crw.c
+OBJ = dcraw.o parse.o fujiturn.o fujiturn16.o fuji_green.o clean_crw.o
+
+# Targets
+default: dcraw$(AB_EXEEXT) dcparse$(AB_EXEEXT) dcfujiturn$(AB_EXEEXT) dcfujiturn16$(AB_EXEEXT) dcfujigreen$(AB_EXEEXT) dccleancrw$(AB_EXEEXT)
+
+dcraw$(AB_EXEEXT): dcraw.o
+	$(CC) -o $@ dcraw.o -lm -ljpeg -llcms -ljasper
+
+dcparse$(AB_EXEEXT): parse.o
+	$(CC) -o $@ parse.o
+
+dcfujiturn$(AB_EXEEXT): fujiturn.o
+	$(CC) -o $@ fujiturn.o
+
+dcfujiturn16$(AB_EXEEXT): fujiturn16.o
+	$(CC) -o $@ fujiturn16.o
+
+dcfujigreen$(AB_EXEEXT): fuji_green.o
+	$(CC) -o $@ fuji_green.o -lm
+
+dccleancrw$(AB_EXEEXT): clean_crw.o
+	$(CC) -o $@ clean_crw.o
+
+clean:
+	rm -f core *.o
+	rm -f dcraw dcparse dcfujiturn dcfujigreen dccleancrw
+	rm -f dcfujiturn16
+
+dcraw.o: dcraw.c
+
+parse.o: parse.c
+
+fixdates.o: fixdates.c
+
+fujiturn.o: fujiturn.c
+
+fujiturn16.o: fujiturn.c
+	$(CC) -c -D_16BIT -o $@ fujiturn.c
+
+fuji_green.o: fuji_green.c
+	$(CC) -c -include netinet/in.h -o $@ fuji_green.c
+
+clean_crw.o: clean_crw.c
+
+install: dcraw dcparse dcfujiturn dcfujiturn16 dcfujigreen dccleancrw
+	$(INSTALL) dcraw $(DESTDIR)/usr/bin
+	$(INSTALL) dcraw.1 $(DESTDIR)/usr/share/man/man1
+	$(INSTALL) dcparse $(DESTDIR)/usr/bin
+	$(INSTALL) dcfujiturn $(DESTDIR)/usr/bin
+	$(INSTALL) dcfujiturn16 $(DESTDIR)/usr/bin
+	$(INSTALL) dcfujigreen $(DESTDIR)/usr/bin
+	$(INSTALL) dccleancrw $(DESTDIR)/usr/bin
