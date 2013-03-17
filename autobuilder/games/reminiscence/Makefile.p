--- Makefile.orig	2009-08-22 08:33:43.000000000 -0700
+++ Makefile	2009-08-22 08:35:53.000000000 -0700
@@ -1,12 +1,12 @@
 
-SDL_CFLAGS = `sdl-config --cflags`
-SDL_LIBS = `sdl-config --libs`
+SDL_CFLAGS = `sdl-config --cflags` -I$GCCSDK_INSTALL_ENV/include
+SDL_LIBS = `sdl-config --libs` -static
 
 DEFINES = -DBYPASS_PROTECTION
 #DEFINES = -DBYPASS_PROTECTION -DNDEBUG
 
-CXX = g++
-CXXFLAGS:= -g -O -Wall -Wuninitialized -Wno-unknown-pragmas -Wshadow -Wimplicit
+#CXX = g++
+CXXFLAGS:= -g -O3 -Wall -Wuninitialized -Wno-unknown-pragmas -Wshadow -Wimplicit
 CXXFLAGS+= -Wundef -Wreorder -Wwrite-strings -Wnon-virtual-dtor -Wno-multichar
 CXXFLAGS+= $(SDL_CFLAGS) $(DEFINES)
 
