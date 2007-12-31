--- clients/Motif-2.1/mwm/Makefile.in.orig	2007-12-30 21:41:45.000000000 -0800
+++ clients/Motif-2.1/mwm/Makefile.in	2007-12-30 21:42:34.000000000 -0800
@@ -306,7 +306,7 @@
 		$(X_LIBS) $(XPLIB) -lXt $(X_PRE_LIBS) -lXext -lX11 $(X_EXTRA_LIBS)
 
 appdir = $(libdir)/X11/app-defaults
-mwmddir = /etc/X11/mwm
+mwmddir = $(GCCSDK_INSTALL_ENV)/etc/X11/mwm
 mwmd_DATA = system.mwmrc alt.map README
 AM_YFLAGS = -d
 all: all-am
