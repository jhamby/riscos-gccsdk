--- Makefile.in.orig	2006-07-09 22:21:58.000000000 -0700
+++ Makefile.in	2006-07-09 22:22:14.000000000 -0700
@@ -163,7 +163,7 @@
 
 INCLUDES = -I${top_srcdir}/include
 
-libXau_la_LDFLAGS = -version-number 6:0:0 -no-undefined
+#libXau_la_LDFLAGS = -version-number 6:0:0 -no-undefined
 
 libXau_la_LIBADD = $(XAU_LIBS)
 
