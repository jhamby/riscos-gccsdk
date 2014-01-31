--- builds/unix/detect.mk.orig	2013-05-28 22:00:03.000000000 +0100
+++ builds/unix/detect.mk	2014-01-04 18:35:52.374143596 +0000
@@ -83,7 +83,7 @@
       # we are building FT2 not in the src tree
 	      $(CONFIG_SHELL) $(TOP_DIR)/builds/unix/configure $(value CFG)
     else
-	      cd builds/unix; $(CONFIG_SHELL) ./configure $(value CFG)
+	      : #cd builds/unix; $(CONFIG_SHELL) ./configure $(value CFG)
     endif
   endif
 
