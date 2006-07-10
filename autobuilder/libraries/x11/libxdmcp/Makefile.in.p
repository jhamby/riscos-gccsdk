--- Makefile.in.orig	2006-07-09 22:24:39.000000000 -0700
+++ Makefile.in	2006-07-09 22:24:58.000000000 -0700
@@ -147,7 +147,7 @@
 lib_LTLIBRARIES = libXdmcp.la
 
 AM_CFLAGS = -I${top_srcdir}/include $(XDMCP_CFLAGS)
-libXdmcp_la_LDFLAGS = -version-number 6:0:0
+#libXdmcp_la_LDFLAGS = -version-number 6:0:0
 libXdmcp_la_LIBADD = $(XDMCP_LIBS)
 libXdmcp_la_SOURCES = \
 	A8Eq.c \
