--- builds/unix/detect.mk.orig	2006-06-25 12:39:04.000000000 -0700
+++ builds/unix/detect.mk	2006-06-25 12:39:27.000000000 -0700
@@ -80,7 +80,7 @@
       # we are building FT2 not in the src tree
 	      $(TOP_DIR)/builds/unix/configure $(CFG)
     else
-	      cd builds/unix; ./configure $(CFG)
+	      : #cd builds/unix; ./configure $(CFG)
     endif
   endif
 
