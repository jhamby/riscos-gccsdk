--- src/Makefile.in.orig	2010-08-18 21:29:01.000000000 +0100
+++ src/Makefile.in	2010-08-18 21:30:20.000000000 +0100
@@ -211,7 +211,7 @@
 sysconfdir = @sysconfdir@
 target_alias = @target_alias@
 INCLUDES = $(DEPS_CFLAGS) -DLOCALEDIR=\""$(datadir)/locale"\" \
-	-DDATADIR=\""$(pkgdatadir)"\"
+	-DDATADIR=\""/<Ceferino\$$Dir>/data"\"

 ceferino_SOURCES = actor.cc audio.cc barra.cc bloque.cc bomba.cc comojugar.cc\
 creditos.cc final.cc fuente.cc gaucho.cc grafico.cc intro.cc item.cc \
