--- Makefile.in.orig	2006-09-16 14:19:47.000000000 -0700
+++ Makefile.in	2006-09-16 14:19:59.000000000 -0700
@@ -217,7 +217,7 @@
 target_alias = @target_alias@
 lib_LTLIBRARIES = libXdmcp.la
 AM_CFLAGS = -I${top_srcdir}/include $(XDMCP_CFLAGS)
-libXdmcp_la_LDFLAGS = -version-number 6:0:0 -no-undefined
+#libXdmcp_la_LDFLAGS = -version-number 6:0:0 -no-undefined
 libXdmcp_la_LIBADD = $(XDMCP_LIBS)
 libXdmcp_la_SOURCES = \
 	A8Eq.c \
