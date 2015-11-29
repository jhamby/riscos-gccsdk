--- builds/unix/detect.mk.orig	2015-11-29 10:28:20.490809352 +0000
+++ builds/unix/detect.mk	2015-11-29 10:28:51.686808659 +0000
@@ -82,8 +82,8 @@
       # we are building FT2 not in the src tree
 	        $(TOP_DIR)/builds/unix/configure $(value CFG)
     else
-	      cd builds/unix; \
-	        ./configure $(value CFG)
+#	      cd builds/unix; \
+#	        ./configure $(value CFG)
     endif
   endif
 
