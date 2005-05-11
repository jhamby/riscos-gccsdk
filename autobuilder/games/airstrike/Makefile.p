--- Makefile.orig	2005-05-11 11:10:56.000000000 +0100
+++ Makefile	2005-05-11 11:11:08.000000000 +0100
@@ -4,7 +4,7 @@
 
 airstrike:
 	$(MAKE) -C src airstrike
-	mv src/airstrike .
+	#mv src/airstrike .
 
 # note: do 'make clean' between sound changing
 airstrike-sound:
