--- builds/unix/detect.mk.orig	2015-10-11 09:48:00.423430738 +0100
+++ builds/unix/detect.mk	2015-10-11 09:50:26.911429148 +0100
@@ -84,9 +84,9 @@
 	      CONFIG_SHELL="$(CONFIG_SHELL)" \
 	        $(CONFIG_SHELL) $(TOP_DIR)/builds/unix/configure $(value CFG)
     else
-	      cd builds/unix; \
-	      CONFIG_SHELL="$(CONFIG_SHELL)" \
-	        $(CONFIG_SHELL) ./configure $(value CFG)
+#	      cd builds/unix; \
+#	      CONFIG_SHELL="$(CONFIG_SHELL)" \
+#	        $(CONFIG_SHELL) ./configure $(value CFG)
     endif
   endif
 
