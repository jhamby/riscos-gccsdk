--- Makefile.orig	2011-01-23 15:43:54.000000000 +0000
+++ Makefile	2011-01-23 15:56:33.000000000 +0000
@@ -44,7 +44,7 @@

 CC=gcc
 LD=gcc
-LDFLAGS=
+LDFLAGS= -static

 # Armulator core endianess of the *emulated* processor (LITTLEEND or BIGEND)
 # Should stay as LITTLEEND when used in ArcEm
@@ -143,8 +143,8 @@
 endif

 ifeq (${SYSTEM},X)
-CFLAGS += -DSYSTEM_X -I/usr/X11R6/include
-LIBS += -L/usr/X11R6/lib -lXext -lX11
+CFLAGS += -DSYSTEM_X -I$(GCCSDK_INSTALL_ENV)/include -mtune=xscale -march=armv5te
+LIBS += -L$(GCCSDK_INSTALL_ENV)/lib -lXext -lCX11 -lDesk
 endif

 ifeq (${SYSTEM},win)
