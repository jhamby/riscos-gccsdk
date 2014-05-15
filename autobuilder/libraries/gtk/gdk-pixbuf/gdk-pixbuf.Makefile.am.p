--- gdk-pixbuf/Makefile.am.orig	2013-12-21 02:53:28.000000000 +0000
+++ gdk-pixbuf/Makefile.am	2014-05-15 17:36:08.396390459 +0100
@@ -519,14 +519,14 @@
 	-I$(top_srcdir)/gdk-pixbuf 					\
 	-I$(top_builddir)/gdk-pixbuf 					\
 	-DGDK_PIXBUF_LOCALEDIR=\"$(localedir)\"				\
-	-DGDK_PIXBUF_LIBDIR=\"$(libdir)\"				\
+	-DGDK_PIXBUF_LIBDIR=\"/SharedLibs:lib/abi-2.0\"			\
 	-DGDK_PIXBUF_BINARY_VERSION=\"$(GDK_PIXBUF_BINARY_VERSION)\"	\
 	-DGDK_PIXBUF_PREFIX=\"$(prefix)\"				\
 	$(INCLUDED_LOADER_DEFINE) 					\
 	$(GDK_PIXBUF_DEBUG_FLAGS)					\
 	$(GDK_PIXBUF_DEP_CFLAGS)					\
 	-DGDK_PIXBUF_ENABLE_BACKEND					\
-	-DPIXBUF_LIBDIR=\"$(loaderdir)\"				\
+	-DPIXBUF_LIBDIR=\"/SharedLibs:lib/abi-2.0/gdk-pixbuf-2.0/$(GDK_PIXBUF_BINARY_VERSION)/loaders\"				\
 	-DBUILT_MODULES_DIR=\"$(srcdir)/.libs\"
 
 LDADDS = libgdk_pixbuf-$(GDK_PIXBUF_API_VERSION).la $(GDK_PIXBUF_DEP_LIBS)
