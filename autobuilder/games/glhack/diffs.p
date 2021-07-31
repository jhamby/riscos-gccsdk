diff -ur include/config.h include/config.h
--- include/config.h	2004-05-28 11:06:59.000000000 +0100
+++ include/config.h	2005-07-06 09:45:37.000000000 +0100
@@ -48,7 +48,7 @@
 /* #define GNOME_GRAPHICS */	/* Gnome interface */
 /* #define MSWIN_GRAPHICS */	/* Windows NT, CE, Graphics */
 #define SDL_GRAPHICS	/* Software SDL graphics (glHack) */
-#define GL_GRAPHICS	/* OpenGL graphics (glHack) */
+/*#define GL_GRAPHICS */	/* OpenGL graphics (glHack) */
 /* #define GL3D_GRAPHICS */	/* OpenGL real 3D renderer (glHack) */
 
 #define VANILLA_GLHACK	/* Differentiates glHack from Slash'EM */
@@ -241,7 +241,7 @@
  * otherwise it will be the current directory.
  */
 # ifndef HACKDIR
-#  define HACKDIR "/usr/lib/games/glhack"
+#  define HACKDIR "<GLHack$Dir>/usr"
 # endif
 
 /*
Only in include: date.h
Only in include: dgn_comp.h
Only in include: lev_comp.h
Only in include: onames.h
Only in include: pm.h
diff -ur sys/share/unixtty.c sys/share/unixtty.c
--- sys/share/unixtty.c	2003-12-07 23:39:13.000000000 +0000
+++ sys/share/unixtty.c	2005-07-06 10:14:31.000000000 +0100
@@ -38,7 +38,7 @@
 # ifdef LINUX
 #  include <sys/ioctl.h>
 #  undef delay_output	/* curses redefines this */
-#  include <curses.h>
+#  include <ncurses/ncurses.h>
 # endif
 # define kill_sym	c_cc[VKILL]
 # define erase_sym	c_cc[VERASE]
diff -ur util/Makefile util/Makefile
--- util/Makefile	2004-05-28 09:00:06.000000000 +0100
+++ util/Makefile	2005-07-06 09:45:37.000000000 +0100
@@ -1,7 +1,7 @@
 #	Makefile for NetHack's utility programs.
 #	SCCS Id: @(#)Makefile.utl	3.4	1997/04/19
 
-CC = gcc
+CC = /usr/bin/gcc
 LFLAGS = -L/usr/X11R6/lib
 YACC = bison -y
 LEX = flex
@@ -141,13 +141,13 @@
 
 # files that define all monsters and objects
 CMONOBJ = ../src/monst.c ../src/objects.c
-OMONOBJ = $(OBJDIR)/monst.o $(OBJDIR)/objects.o
+OMONOBJ = $(OBJDIR)/monstnative.o $(OBJDIR)/objectsnative.o
 # files that provide access to NetHack's names
 CNAMING = ../src/drawing.c ../src/decl.c $(CMONOBJ)
-ONAMING = $(OBJDIR)/drawing.o $(OBJDIR)/decl.o $(OMONOBJ)
+ONAMING = $(OBJDIR)/drawingnative.o $(OBJDIR)/declnative.o $(OMONOBJ)
 # dynamic memory allocation
 CALLOC = ../src/alloc.c panic.c
-OALLOC = $(OBJDIR)/alloc.o panic.o
+OALLOC = $(OBJDIR)/allocnative.o panic.o
 
 # object files for makedefs
 MAKEOBJS = makedefs.o $(OMONOBJ)
@@ -162,7 +162,7 @@
 RECOVOBJS = recover.o
 
 # object files for the data librarian
-DLBOBJS = dlb_main.o $(OBJDIR)/dlb.o $(OALLOC)
+DLBOBJS = dlb_main.o $(OBJDIR)/dlbnative.o $(OALLOC)
 
 # flags for creating distribution versions of sys/share/*_lex.c, using
 # a more portable flex skeleton, which is not included in the distribution.
@@ -371,17 +371,17 @@
 
 # make sure object files from src are available when needed
 #
-$(OBJDIR)/alloc.o: ../src/alloc.c $(CONFIG_H)
+$(OBJDIR)/allocnative.o: ../src/alloc.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/alloc.c -o $@
-$(OBJDIR)/drawing.o: ../src/drawing.c $(CONFIG_H)
+$(OBJDIR)/drawingnative.o: ../src/drawing.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/drawing.c -o $@
-$(OBJDIR)/decl.o: ../src/decl.c $(CONFIG_H)
+$(OBJDIR)/declnative.o: ../src/decl.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/decl.c -o $@
-$(OBJDIR)/monst.o: ../src/monst.c $(CONFIG_H)
+$(OBJDIR)/monstnative.o: ../src/monst.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/monst.c -o $@
-$(OBJDIR)/objects.o: ../src/objects.c $(CONFIG_H)
+$(OBJDIR)/objectsnative.o: ../src/objects.c $(CONFIG_H)
 	$(CC) $(CFLAGS) -c ../src/objects.c -o $@
-$(OBJDIR)/dlb.o: ../src/dlb.c $(HACK_H) ../include/dlb.h
+$(OBJDIR)/dlbnative.o: ../src/dlb.c $(HACK_H) ../include/dlb.h
 	$(CC) $(CFLAGS) -c ../src/dlb.c -o $@
 
 # make sure hack.h dependencies get transitive information
