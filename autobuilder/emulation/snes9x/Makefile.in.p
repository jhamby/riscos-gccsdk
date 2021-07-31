--- sdl/Makefile.in.orig	2012-04-10 15:38:14.000000000 +0100
+++ sdl/Makefile.in	2012-04-10 15:39:04.000000000 +0100
@@ -46,7 +46,7 @@
 	exit 1
 
 snes9x-sdl: $(OBJECTS)
-	$(CCC) $(INCLUDES) -o $@ $(OBJECTS) -lm @S9XLIBS@
+	$(CCC) -static $(INCLUDES) -o $@ $(OBJECTS) -lm @S9XLIBS@
 
 ../jma/s9x-jma.o: ../jma/s9x-jma.cpp
 	$(CCC) $(INCLUDES) -c $(CCFLAGS) -fexceptions $*.cpp -o $@
