--- Makefile.old	2003-04-25 08:34:23.000000000 +0100
+++ Makefile	2003-04-25 07:39:08.000000000 +0100
@@ -113,6 +113,10 @@
 generic:
 	make -e $(ALL) $(MFLAGS) XFLAGS='-DGENERIC' STATIC=
 
+# RISC OS by Peter Naulls
+riscos:
+	make -e $(ALL) $(MFLAGS) XFLAGS='-DRISOS -O2' STATIC=-static
+
 # Still at large: dgux dynix ???
 
 ### RANDOM
