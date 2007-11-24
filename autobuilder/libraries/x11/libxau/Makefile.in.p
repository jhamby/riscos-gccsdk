--- Makefile.in.orig	2007-11-24 11:47:42.000000000 -0800
+++ Makefile.in	2007-11-24 11:48:03.000000000 -0800
@@ -245,7 +245,7 @@
 lib_LTLIBRARIES = libXau.la
 AM_CFLAGS = $(XAU_CFLAGS)
 INCLUDES = -I${top_srcdir}/include
-libXau_la_LDFLAGS = -version-number 6:0:0 -no-undefined
+#libXau_la_LDFLAGS = -version-number 6:0:0 -no-undefined
 libXau_la_LIBADD = $(XAU_LIBS)
 libXau_la_SOURCES = AuDispose.c AuFileName.c AuGetAddr.c AuGetBest.c \
 	AuLock.c AuRead.c AuUnlock.c AuWrite.c $(am__append_1)
