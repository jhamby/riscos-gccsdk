--- src/Makefile.orig	2021-08-08 08:49:03.515028862 +0100
+++ src/Makefile	2021-08-08 08:49:39.134363458 +0100
@@ -18,7 +18,7 @@
 export CFLAGS LDFLAGS
 
 angband: angband.o $(MAINFILES)
-	$(CC) -o angband angband.o $(MAINFILES) $(LDFLAGS) $(LDADD)
+	$(CC) -o angband angband.o $(MAINFILES) $(LDFLAGS) $(LDADD) -lpng -lz -lSDL -ljpeg -lwebp -lfreetype -lbz2 -lFLAC -lmikmod -lvorbis -lvorbisidec -logg
 	@printf "%10s %-20s\n" LINK $@
 
 angband.o: $(OBJECTS)
