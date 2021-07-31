--- gdk/Makefile.am.orig	2013-10-17 00:28:05.000000000 +0100
+++ gdk/Makefile.am	2014-05-11 20:18:40.565274545 +0100
@@ -57,6 +57,11 @@
 	$(no_undefined)				\
 	$(LIBTOOL_EXPORT_OPTIONS)
 
+if USE_RISCOS
+LDADD +=	\
+	-lOSLib32
+endif
+
 #
 # setup source file variables
 #
@@ -191,6 +196,10 @@
 libgdk_3_la_LIBADD += wayland/libgdk-wayland.la
 endif
 
+if USE_RISCOS
+libgdk_3_la_LIBADD += riscos/libgdk-riscos.la
+endif
+
 if HAVE_INTROSPECTION
 
 introspection_files = 		\
