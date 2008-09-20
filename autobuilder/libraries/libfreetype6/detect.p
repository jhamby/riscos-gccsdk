--- builds/unix/detect.mk.orig	2007-12-06 16:21:03.000000000 +0000
+++ builds/unix/detect.mk	2008-09-15 20:25:49.000000000 +0100
@@ -81,7 +81,7 @@
       # we are building FT2 not in the src tree
 	      $(TOP_DIR)/builds/unix/configure $(value CFG)
     else
-	      cd builds/unix; ./configure $(value CFG)
+	      : #cd builds/unix; ./configure $(value CFG)
     endif
   endif
 
