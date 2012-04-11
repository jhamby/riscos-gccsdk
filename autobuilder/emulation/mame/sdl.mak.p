--- src/osd/sdl/sdl.mak.orig	2012-04-09 14:34:13.000000000 +0100
+++ src/osd/sdl/sdl.mak	2012-04-09 14:37:58.000000000 +0100
@@ -31,12 +31,12 @@
 
 # uncomment next line to build without OpenGL support
 
-# NO_OPENGL = 1
+NO_OPENGL = 1
 
 # uncomment next line to build without X11 support (TARGETOS=unix only)
 # this also implies, that no debugger will be builtin.
 
-# NO_X11 = 1
+NO_X11 = 1
 
 # uncomment and adapt next line to link against specific GL-Library
 # this will also add a rpath to the executable
@@ -46,7 +46,7 @@
 # GL-dispatching.
 # This option takes precedence over MESA_INSTALL_ROOT
 
-USE_DISPATCH_GL = 1
+#USE_DISPATCH_GL = 1
 
 # uncomment and change the next line to compile and link to specific
 # SDL library. This is currently supported for unix and win32.
@@ -83,6 +83,10 @@
 
 ifeq ($(DISTRO),)
 DISTRO = generic
+DEFS += -DNO_AFFINITY_NP
+DEFS += -DSDLMAME_NO64BITIO
+LDFLAGS += -static
+LIBS += -lfreetype -lbz2 -lstdc++
 else
 ifeq ($(DISTRO),debian-stable)
 DEFS += -DNO_AFFINITY_NP
@@ -378,7 +378,7 @@
 
 INCPATH += `pkg-config --cflags fontconfig`
 LIBS += `pkg-config --libs fontconfig`
-LIBS += -lSDL_ttf -lutil
+LIBS += -lSDL_ttf
 
 endif # Unix
 
