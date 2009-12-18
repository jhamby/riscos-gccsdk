--- x11/Makefile.in.orig	2009-08-26 13:37:30.000000000 -0700
+++ x11/Makefile.in	2009-08-26 13:37:42.000000000 -0700
@@ -39,13 +39,13 @@
 
 CC = @CC@
 
-CFLAGS = -g
+CFLAGS = -O2
 
 LDFLAGS =
 
-X11_LIB = -lXext -lXmu -lX11
+X11_LIB = -lXext -lXmu -lCX11 -lDesk -static
 
-XAW_LIB = -lXaw -lXt $(X11_LIB) -lXext -lm
+XAW_LIB = -lXaw3d -lXt $(X11_LIB) -lXext -lm
 
 #TCLTK_LIB = -ltk -ltcl -ldl -lm
 #TCLTK_LIB = ../tcl/unix/libtcl8.0.a ../tk/unix/libtk8.0.a -ldl -lm
@@ -247,4 +247,4 @@
 
 xconq.o:	$(ALL_H) $(krnsrcdir)/cmdline.h $(srcdir)/../tcltk/tkconq.h
 
-ximfapp.o:	$(LOW_H) $(krnsrcdir)/imf.h
\ No newline at end of file
+ximfapp.o:	$(LOW_H) $(krnsrcdir)/imf.h
--- kernel/Makefile.in.orig	2009-12-08 09:26:26.000000000 -0800
+++ kernel/Makefile.in	2009-12-08 09:38:27.000000000 -0800
@@ -11,7 +11,7 @@
 exec_prefix = $(prefix)
 bindir = $(exec_prefix)/bin
 libdir = $(exec_prefix)/lib
-datadir = $(prefix)/share/xconq
+datadir = "/Xconq:/data"
 mandir = $(prefix)/man
 man6dir = $(mandir)/man6
 infodir = $(prefix)/info
@@ -114,7 +114,7 @@
 # kernel, since the values get bound into low-level routines in unix.c
 # and win32.c.
 
-ALL_CFLAGS = $(CFLAGS) $(HFLAGS) -DXCONQDATA=\"$(datadir)\" -DXCONQSCORES=\"$(scoresdir)\" -I$(srcdir)
+ALL_CFLAGS = $(CFLAGS) $(HFLAGS) -DXCONQDATA=\"$(datadir)\" -DXCONQSCORES="\"/<Choices\$$Write>/Xconq/\"" -I$(srcdir)
 
 .c.o:
 	$(CC) -c $(ALL_CFLAGS) $<
