--- build/top.mk.orig	2010-06-05 09:25:14.097220336 +0100
+++ build/top.mk	2010-06-05 09:25:29.904755578 +0100
@@ -105,7 +105,7 @@
 LINK_LIBS = $(MOD_LIBS) $(@PLAT@_LINK_LIBS)
 
 # option to pass to $(CC) to support library symbol versioning, if any
-VERSION_OPTION = @VERSION_OPTION@
+VERSION_OPTION = 
 
 LTSTATIC = @LTSTATIC@
 
