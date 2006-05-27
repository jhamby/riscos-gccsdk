--- src/contrib.mak.org	2002-10-17 01:26:34.000000000 +0200
+++ src/contrib.mak	2006-05-28 00:56:01.000000000 +0200
@@ -850,3 +850,40 @@
 $(GLOBJ)gdevsunr.$(OBJ) : $(GLSRC)gdevsunr.c $(PDEVH)
 	$(GLCC) $(GLO_)gdevsunr.$(OBJ) $(C_) $(GLSRC)gdevsunr.c
 
+### ---------------- The Samsung SmartGDI laser printer devices --------- ###
+### Note : this driver is used by Samsung SmartGDI compatible printers.   ###
+###
+
+GDIMONO=$(GLOBJ)gdevgdi.$(OBJ) $(HPPCL)
+
+$(DD)gdi.dev: $(GDIMONO) $(DD)page.dev
+	$(SETPDEV) $(DD)gdi $(GDIMONO)
+
+$(DD)samsunggdi.dev: $(GDIMONO) $(DD)page.dev
+	$(SETPDEV) $(DD)samsunggdi $(GDIMONO)
+
+$(GLOBJ)gdevgdi.$(OBJ): $(GLSRC)gdevgdi.c $(PDEVH) $(gdevpcl_h)
+	$(GLCC) $(GLO_)gdevgdi.$(OBJ) $(C_) $(GLSRC)gdevgdi.c
+
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
+$(GLOBJ)gdevspr.$(OBJ) : $(GLSRC)gdevspr.c $(PDEVH)
+	$(GLCC) $(GLO_)gdevspr.$(OBJ) $(C_) $(GLSRC)gdevspr.c
+
