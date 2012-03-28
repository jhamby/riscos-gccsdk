--- Makefile.in.orig	2012-03-28 11:12:45.000000000 +0100
+++ Makefile.in	2012-03-28 11:13:24.000000000 +0100
@@ -327,7 +327,7 @@
 	  z80

 DIST_SUBDIRS = debugger disk hacking ide lib machines man perl \
-	pokefinder roms sound timer ui unittests z80 compat/amiga \
+	pokefinder sound timer ui unittests compat/amiga \
 	compat/morphos compat/unix compat/wii compat/win32
 fuse_SOURCES = ay.c dck.c display.c event.c fuller.c fuse.c if1.c \
 	if2.c input.c joystick.c kempmouse.c keyboard.c loader.c \
@@ -377,7 +377,7 @@

 BUILT_SOURCES = options.h settings.c settings.h
 INCLUDES = @GLIB_CFLAGS@ @GTK_CFLAGS@ @LIBSPEC_CFLAGS@ \
-@XML_CFLAGS@ -DFUSEDATADIR="\"${pkgdatadir}\"" @SDL_CFLAGS@
+@XML_CFLAGS@ -DFUSEDATADIR="\"/<Fuse\$$Dir>/\"" @SDL_CFLAGS@

 noinst_HEADERS = ay.h bitmap.h compat.h dck.h display.h event.h \
 	fuller.h fuse.h if1.h if2.h input.h keyboard.h kempmouse.h \
