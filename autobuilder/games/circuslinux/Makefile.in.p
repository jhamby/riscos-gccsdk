--- Makefile.in.old	2003-04-27 20:52:08.000000000 +0100
+++ Makefile.in	2003-04-27 19:52:20.000000000 +0100
@@ -81,10 +81,11 @@
 
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
 
