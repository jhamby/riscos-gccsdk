--- Makefile.in.orig	2001-04-29 07:53:31.000000000 +0100
+++ Makefile.in	2012-10-12 12:49:41.000000000 +0100
@@ -81,11 +81,12 @@
 
 # User-definable stuff:
 CC = @CC@
-DATA_PREFIX = $(pkgdatadir)/data/
+#DATA_PREFIX = $(pkgdatadir)/data/
+DATA_PREFIX= \<CircusLinux\$$Dir\>\/data/
 JOY = @JOY@
-TARGET_DEF = @TARGET_DEF@
+TARGET_DEF = RISCOS
 
-CFLAGS = @CFLAGS@ -Wall -O2 -DDATA_PREFIX=\"$(DATA_PREFIX)\" 	  -D$(TARGET_DEF) -DJOY_$(JOY)
+CFLAGS = @CFLAGS@ -Wall -O2 -DDATA_PREFIX=\"$(DATA_PREFIX)\" 	  -D$(TARGET_DEF) -DJOY_$(JOY) -DSHARED_SCOREFILE_NO
 
 LIBS = @LIBS@ 
