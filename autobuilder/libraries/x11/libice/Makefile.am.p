--- src/Makefile.am.orig	2006-10-17 01:26:44.000000000 +0200
+++ src/Makefile.am	2006-10-17 01:26:55.000000000 +0200
@@ -3,7 +3,7 @@
 AM_CFLAGS= -I${top_srcdir}/include $(ICE_CFLAGS) $(XTRANS_CFLAGS) \
 	-DICE_t -DTRANS_CLIENT -DTRANS_SERVER
 
-libICE_la_LDFLAGS = -version-number 6:3:0 -no-undefined
+#libICE_la_LDFLAGS = -version-number 6:3:0 -no-undefined
 
 libICE_la_LIBADD = $(ICE_LIBS) $(XTRANS_LIBS)
 
