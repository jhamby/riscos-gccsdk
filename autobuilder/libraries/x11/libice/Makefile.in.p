--- src/Makefile.in.orig	2006-09-16 14:36:22.000000000 -0700
+++ src/Makefile.in	2006-09-16 14:36:39.000000000 -0700
@@ -190,7 +190,7 @@
 AM_CFLAGS = -I${top_srcdir}/include $(ICE_CFLAGS) $(XTRANS_CFLAGS) \
 	-DICE_t -DTRANS_CLIENT -DTRANS_SERVER
 
-libICE_la_LDFLAGS = -version-number 6:3:0 -no-undefined
+#libICE_la_LDFLAGS = -version-number 6:3:0 -no-undefined
 libICE_la_LIBADD = $(ICE_LIBS) $(XTRANS_LIBS)
 libICE_la_SOURCES = \
 	ICElibint.h \
