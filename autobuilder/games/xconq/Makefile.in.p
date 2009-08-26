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
