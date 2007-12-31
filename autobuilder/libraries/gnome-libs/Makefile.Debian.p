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
