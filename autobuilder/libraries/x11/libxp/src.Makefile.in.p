--- src/Makefile.in.org	2005-08-25 10:29:36.000000000 +0200
+++ src/Makefile.in	2006-08-03 01:26:29.000000000 +0200
@@ -191,7 +191,8 @@
 SUBDIRS = util
 lib_LTLIBRARIES = libXp.la
 AM_CFLAGS = $(XP_CFLAGS) -I$(top_srcdir)/include -D_BSD_SOURCE
-libXp_la_LDFLAGS = -version-number 6:2:1
+#libXp_la_LDFLAGS = -version-number 6:2:1
+libXp_la_LDFLAGS =
 libXp_la_SOURCES = \
                    XpAttr.c \
                    XpContext.c \
