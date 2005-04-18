--- Makefile.orig	2005-04-18 10:14:24.000000000 +0100
+++ Makefile	2005-04-18 10:14:31.000000000 +0100
@@ -26,7 +26,7 @@
 
 # remove debug information from binaries
 strip:
-	$(STRIP) bin/*
+	#$(STRIP) bin/*
 
 # official installation
 install: all strip
