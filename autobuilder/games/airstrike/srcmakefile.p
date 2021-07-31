--- src/Makefile.orig	2021-03-07 09:39:29.353799707 +0000
+++ src/Makefile	2021-03-07 09:40:10.293225237 +0000
@@ -1,12 +1,11 @@
 CFLAGS:= -g -O2 -Wall -Wno-unused $(OPTIONS)
 #
 CFLAGS += $(shell sdl-config --cflags) -Isprite -I. -Isupport -DINLINE=inline
-CFLAGS += `dpkg-buildflags --get CFLAGS`
 CFILES:= $(sort $(shell find . -name '*.c'))
 OBJECTS:= $(CFILES:.c=.o)
 
 airstrike: $(OBJECTS)
-	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lm
+	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lm -ljpeg -lpng -lwebp -lz -ltiff -llzma -static
 
 airstrike-sound: $(OBJECTS)
 	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lSDL_mixer
