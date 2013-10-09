--- Makefile.orig	2009-08-19 16:33:04.000000000 -0700
+++ Makefile	2009-08-19 16:34:00.000000000 -0700
@@ -22,7 +22,7 @@
 	cd modify && make
 
 $(TARGET): $(OBJS) $(SDL_TARGET) data globals.h
-	$(CC) -o $(TARGET) $(OBJS) $(LIBS) $(SDL_TARGET)
+	$(CC) -o $(TARGET) $(OBJS) $(LIBS) $(SDL_TARGET) -lvorbisidec -logg -lmikmod -static
 
 $(OBJS): globals.h
 
