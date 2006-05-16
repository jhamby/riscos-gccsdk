--- src/contrib.mak.org	2002-10-17 01:26:34.000000000 +0200
+++ src/contrib.mak	2006-05-15 19:03:01.460000000 +0200
@@ -850,3 +850,25 @@
 $(GLOBJ)gdevsunr.$(OBJ) : $(GLSRC)gdevsunr.c $(PDEVH)
 	$(GLCC) $(GLO_)gdevsunr.$(OBJ) $(C_) $(GLSRC)gdevsunr.c
 
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
