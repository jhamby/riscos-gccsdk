--- Makefile.orig	2013-06-02 19:37:59.066120262 +0100
+++ Makefile	2013-06-02 19:38:58.406013812 +0100
@@ -1,13 +1,13 @@
 
 SDL_CFLAGS = `sdl-config --cflags`
-SDL_LIBS = `sdl-config --libs`
+SDL_LIBS = `sdl-config --libs` -static
 
 DEFINES = -DBYPASS_PROTECTION
 #DEFINES = -DBYPASS_PROTECTION -DENABLE_PASSWORD_MENU -DNDEBUG
 
 CXX = g++
 EXT_CXXFLAGS := $(CXXFLAGS)
-CXXFLAGS := -g -O -Wall -Wuninitialized -Wshadow -Wimplicit -Wundef -Wreorder -Wnon-virtual-dtor -Wno-multichar
+CXXFLAGS := -g -O3 -Wall -Wuninitialized -Wshadow -Wimplicit -Wundef -Wreorder -Wnon-virtual-dtor -Wno-multichar
 CXXFLAGS += -MMD $(SDL_CFLAGS) -DUSE_ZLIB $(DEFINES) $(EXT_CXXFLAGS)
 
 SRCS = collision.cpp cutscene.cpp file.cpp fs.cpp game.cpp graphics.cpp main.cpp menu.cpp \
