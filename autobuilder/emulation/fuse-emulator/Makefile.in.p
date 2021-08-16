--- Makefile.in.orig	2021-08-05 13:14:26.071390879 +0100
+++ Makefile.in	2021-08-05 13:16:28.992985072 +0100
@@ -897,7 +897,7 @@
 	$(am__append_39) $(am__append_41) z80/opcodes_base.c \
 	z80/z80_cb.c z80/z80_ddfd.c z80/z80_ddfdcb.c z80/z80_ed.c
 AM_CPPFLAGS = $(GLIB_CFLAGS) $(GTK_CFLAGS) $(LIBSPECTRUM_CFLAGS) \
-	$(XML_CFLAGS) -DFUSEDATADIR="\"${pkgdatadir}\"" $(PNG_CFLAGS) \
+	$(XML_CFLAGS) -DFUSEDATADIR="\"/<Fuse\$Dir>\"" $(PNG_CFLAGS) \
 	$(am__append_2)
 AM_CFLAGS = $(WARN_CFLAGS) $(PTHREAD_CFLAGS)
 noinst_HEADERS = bitmap.h compat.h display.h event.h fuse.h input.h \
