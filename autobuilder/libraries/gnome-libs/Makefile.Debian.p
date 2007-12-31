--- libgnorba/Makefile.in.orig	2007-12-30 16:48:59.000000000 -0800
+++ libgnorba/Makefile.in	2007-12-30 16:51:09.000000000 -0800
@@ -268,7 +268,7 @@
 	libgnorbagtk.la					\
 	-lname-server					\
 	$(ORBIT_LIBS) 					\
-	$(X_LIBS) -lX11	-lXext -lXi \
+	$(X_LIBS) -lCX11 -lDesk -lXext -lXi \
 	$(INTLLIBS)
 
 
@@ -297,7 +297,7 @@
 goad.lo orbitgtk.lo orbitgnome.lo orbitns.lo
 libgnorbagtk_la_LDFLAGS = 
 libgnorbagtk_la_LIBADD = 
-libgnorbagtk_la_LIBADD += $(ORBIT_LIBS) $(GLIB_LIBS) -lm -lgdk -ldl -lgtk -lgmodule -lX11 -lpopt
+libgnorbagtk_la_LIBADD += $(ORBIT_LIBS) $(GLIB_LIBS) -lm -lgdk -ldl -lgtk -lgmodule -lCX11 -lDesk -lpopt
 libgnorbagtk_la_OBJECTS =  gnome-factory-skels.lo gnome-factory-stubs.lo \
 gnome-factory-common.lo Table-skels.lo Table-stubs.lo Table-common.lo \
 gnome-unknown-skels.lo gnome-unknown-stubs.lo gnome-unknown-common.lo \
--- libgnome/Makefile.in.orig	2007-12-31 12:53:16.000000000 -0800
+++ libgnome/Makefile.in	2007-12-31 12:53:41.000000000 -0800
@@ -256,7 +256,7 @@
 
 gnome_moz_remote_SOURCES = gnome-moz-remote.c vroot.h
 gnome_moz_remote_LDADD = libgnome.la $(LIBSUPPORT) $(LIBGNOME_LIBS) \
-	$(X_LIBS) $(X_PRE_LIBS) -lXext -lX11 $(X_EXTRA_LIBS) $(INTLLIBS)
+	$(X_LIBS) $(X_PRE_LIBS) -lXext -lCX11 -lDesk $(X_EXTRA_LIBS) $(INTLLIBS)
 
 
 gconfigger_SOURCES = gconfigger.c
--- zvt/Makefile.in.orig	2007-12-31 12:53:56.000000000 -0800
+++ zvt/Makefile.in	2007-12-31 12:54:17.000000000 -0800
@@ -222,7 +222,7 @@
 X_EXTRA_LIBS = @X_EXTRA_LIBS@
 X_PRE_LIBS = @X_PRE_LIBS@
 libzvt_la_LIBADD = 
-libzvt_la_LIBADD += -lgdk -lgmodule -lX11 -lgtk -lgdk_imlib -lutil
+libzvt_la_LIBADD += -lgdk -lgmodule -lCX11 -lDesk -lgtk -lgdk_imlib -lutil
 libzvt_la_OBJECTS =  gnome-login-support.lo lists.lo subshell.lo \
 update.lo vt.lo zvtterm.lo
 noinst_PROGRAMS =  zterm$(EXEEXT) test-utmp$(EXEEXT)
