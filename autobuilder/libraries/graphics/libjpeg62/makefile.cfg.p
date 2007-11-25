--- makefile.cfg.orig	2007-11-25 10:03:22.000000000 -0800
+++ makefile.cfg	2007-11-25 10:04:10.000000000 -0800
@@ -174,19 +174,19 @@
 # sample programs:
 
 cjpeg: $(COBJECTS) libjpeg.$(A)
-	$(LN) $(LDFLAGS) -o cjpeg $(COBJECTS) libjpeg.$(A) $(LDLIBS)
+	$(LN) $(LDFLAGS) -o cjpeg$(AB_EXEEXT) $(COBJECTS) libjpeg.$(A) $(LDLIBS)
 
 djpeg: $(DOBJECTS) libjpeg.$(A)
-	$(LN) $(LDFLAGS) -o djpeg $(DOBJECTS) libjpeg.$(A) $(LDLIBS)
+	$(LN) $(LDFLAGS) -o djpeg$(AB_EXEEXT) $(DOBJECTS) libjpeg.$(A) $(LDLIBS)
 
 jpegtran: $(TROBJECTS) libjpeg.$(A)
-	$(LN) $(LDFLAGS) -o jpegtran $(TROBJECTS) libjpeg.$(A) $(LDLIBS)
+	$(LN) $(LDFLAGS) -o jpegtran$(AB_EXEEXT) $(TROBJECTS) libjpeg.$(A) $(LDLIBS)
 
 rdjpgcom: rdjpgcom.$(O)
-	$(LN) $(LDFLAGS) -o rdjpgcom rdjpgcom.$(O) $(LDLIBS)
+	$(LN) $(LDFLAGS) -o rdjpgcom$(AB_EXEEXT) rdjpgcom.$(O) $(LDLIBS)
 
 wrjpgcom: wrjpgcom.$(O)
-	$(LN) $(LDFLAGS) -o wrjpgcom wrjpgcom.$(O) $(LDLIBS)
+	$(LN) $(LDFLAGS) -o wrjpgcom$(AB_EXEEXT) wrjpgcom.$(O) $(LDLIBS)
 
 # Installation rules:
 
