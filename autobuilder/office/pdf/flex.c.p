--- GuiLib/GuiFlex/flex.c.orig	2010-05-10 20:53:16.671682507 +0100
+++ GuiLib/GuiFlex/flex.c	2010-05-10 20:54:49.731718896 +0100
@@ -862,11 +862,14 @@
 
 extern int flex_set_budge(int newstate)
 {
-    _kernel_ExtendProc *oldproc;
 
     if (flex__area_num)
         return -1;
 
+	return 0;
+/*
+    _kernel_ExtendProc *oldproc;
+
     if (newstate == 0)
         oldproc = _kernel_register_slotextend(flex_dont_budge);
     else if (newstate == 1)
@@ -881,7 +884,7 @@
     else if (oldproc == flex_dont_budge)
         return 0;
     else
-        return (int) oldproc;
+        return (int) oldproc; */
 }
 
 
