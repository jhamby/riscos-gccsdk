--- lib/Makefile.in.org	2007-11-30 19:59:58.000000000 -0800
+++ lib/Makefile.in	2007-11-30 20:00:28.000000000 -0800
@@ -418,11 +418,11 @@
 newnumber.o: number.c muldigits.h
 	$(CC) $(CFLAGS) $(INCLUDES) -c -DMULDIGITS -o newnumber.o $(srcdir)/number.c
 
-muldigits.h: testmul
+muldigits.h: testmul$(EXEEXT)
 	@echo "The following may take up to 10 minutes."
 	./testmul > muldigits.h
 
-testmul: testmul.o number.o
+testmul$(EXEEXT): testmul.o number.o
 	$(CC) $(CFLAGS) -o testmul testmul.o number.o
 
 specialnumber: newnumber.o
