--- contrib/contrib.mak.orig	2009-12-07 19:26:09.000000000 -0800
+++ contrib/contrib.mak	2009-12-07 19:27:49.000000000 -0800
@@ -256,6 +256,28 @@
 	$(GLCC) $(GLO_)gdevgdi.$(OBJ) $(C_) $(CONTRIBSRC)gdevgdi.c


+### ---------------- RISC OS Sprite files ---------------- ###
+
+spr_=$(GLOBJ)gdevspr.$(OBJ)
+
+$(DD)spr.dev : $(spr_) $(DD)page.dev
+	$(SETPDEV) $(DD)spr $(spr_)
+
+$(DD)spr1.dev : $(spr_) $(DD)page.dev
+	$(SETPDEV) $(DD)spr1 $(spr_)
+
+$(DD)spr8c.dev : $(spr_) $(DD)page.dev
+	$(SETPDEV) $(DD)spr8c $(spr_)
+
+$(DD)spr8m.dev : $(spr_) $(DD)page.dev
+	$(SETPDEV) $(DD)spr8m $(spr_)
+
+$(DD)sprm.dev : $(spr_) $(DD)page.dev
+	$(SETPDEV) $(DD)sprm $(spr_)
+
+$(GLOBJ)gdevspr.$(OBJ) : $(CONTRIBSRC)gdevspr.c $(PDEVH)
+	$(GLCC) $(GLO_)gdevspr.$(OBJ) $(C_) $(CONTRIBSRC)gdevspr.c
+
 ### ---------------- The Brother HL-1240/HL-1250 printers --------------- ###
 ### These printers are ljet4-compatible, plus support for source tray     ###
 ### selection and special 1200x600 dpi mode.                              ###
