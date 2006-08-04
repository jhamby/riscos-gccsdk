--- src/util/Makefile.in.org	2005-08-25 10:29:36.000000000 +0200
+++ src/util/Makefile.in	2006-08-03 01:21:44.000000000 +0200
@@ -180,7 +180,8 @@
 target_alias = @target_alias@
 lib_LTLIBRARIES = libXprintUtil.la
 AM_CFLAGS = $(XP_CFLAGS) -I$(top_srcdir)/include
-libXprintUtil_la_LDFLAGS = -version-number 6:2:0
+#libXprintUtil_la_LDFLAGS = -version-number 6:2:0
+libXprintUtil_la_LDFLAGS =
 libXprintUtil_la_SOURCES = \
                            xprintutil.c \
                            xprintutil_printtofile.c
