--- snes9x-1.42-src/snes9x/Makefile.in.old	2005-07-10 10:29:56.000000000 +0100
+++ snes9x-1.42-src/snes9x/Makefile.in	2005-07-10 10:30:26.000000000 +0100
@@ -154,7 +154,7 @@
 #OPTIMISE=-g -fno-exceptions
 #-DMITSHM 
 
-CCFLAGS = $(OPTIMISE) -DMITSHM \
+CCFLAGS = $(OPTIMISE) \
 @CPUINC@ -I. \
 -Iunzip \
 -DVAR_CYCLES \
@@ -186,7 +186,7 @@
 CFLAGS=$(CCFLAGS)
 
 .SUFFIXES: .o .cpp .c .cc .h .m .i .S .asm .obj .O .CPP .C .ASM
-LDLIBS = -L/usr/X11R6/lib -L../zlib
+LDLIBS = -L/home/riscos/env/lib -L../zlib
 
 ifdef GLIDE
 all: offsets gsnes9x
--- snes9x-1.42-src/snes9x/Makefile.in.orig	2005-07-10 10:55:33.000000000 +0100
+++ snes9x-1.42-src/snes9x/Makefile.in	2005-07-10 10:55:37.000000000 +0100
@@ -242,7 +242,7 @@
 	$(CCC) $(INCLUDES) -o $@ $(OBJECTS) unix/x11.o unix/linux-x11_key.o $(OPENGLOBJS) $(LDLIBS) $(OPENGLLIBS) @SYSLIBS@ -lXext -lX11 $(EXTRALIBS) -lz -lm
 
 unix/compile-keymap: unix/compile-keymap.o unix/keymap.o
-	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $^ -lz
+	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $^ -lz -L/home/riscos/env/lib
 
 unix/linux-x11_key.c: unix/compile-keymap unix/linux-x11.key
 	unix/compile-keymap unix/linux-x11.key unix/linux-x11_key.c
