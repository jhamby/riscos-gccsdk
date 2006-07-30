--- src/Makefile.in.org	2006-07-30 07:18:49.000000000 -0700
+++ src/Makefile.in	2006-07-30 07:19:05.000000000 -0700
@@ -144,7 +144,7 @@
 	-DICE_t -DTRANS_CLIENT -DTRANS_SERVER
 
 
-libICE_la_LDFLAGS = -version-number 6:3:0 -no-undefined
+#libICE_la_LDFLAGS = -version-number 6:3:0 -no-undefined
 
 libICE_la_LIBADD = $(ICE_LIBS) $(XTRANS_LIBS)
 
