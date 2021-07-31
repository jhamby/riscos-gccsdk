--- linc2/src/Makefile.in.orig	2013-05-29 20:43:25.640247917 +0100
+++ linc2/src/Makefile.in	2013-05-29 20:43:44.112230754 +0100
@@ -244,7 +244,7 @@
 #	-I$(top_srcdir)/include    
 INCLUDES = -I$(top_builddir)/linc2/include \
 	-I$(top_srcdir)/linc2/include $(LINC_CFLAGS) $(WARN_CFLAGS) \
-	-DG_DISABLE_DEPRECATED $(am__append_1)
+	$(am__append_1)
 liblinc_la_SOURCES = \
 	linc.c			\
 	linc-connection.c	\
