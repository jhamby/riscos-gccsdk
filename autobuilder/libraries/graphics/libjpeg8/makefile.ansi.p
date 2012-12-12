--- makefile.ansi.orig	2012-03-08 09:26:17.685857505 +0000
+++ makefile.ansi	2012-03-08 09:26:56.842107376 +0000
@@ -111,19 +111,19 @@
 	$(AR2) libjpeg.a
 
 cjpeg: $(COBJECTS) libjpeg.a
-	$(LN) $(LDFLAGS) -o cjpeg $(COBJECTS) libjpeg.a $(LDLIBS)
+	$(LN) $(LDFLAGS) -o cjpeg$(AB_EXEEXT) $(COBJECTS) libjpeg.a $(LDLIBS)
 
 djpeg: $(DOBJECTS) libjpeg.a
-	$(LN) $(LDFLAGS) -o djpeg $(DOBJECTS) libjpeg.a $(LDLIBS)
+	$(LN) $(LDFLAGS) -o djpeg$(AB_EXEEXT) $(DOBJECTS) libjpeg.a $(LDLIBS)
 
 jpegtran: $(TROBJECTS) libjpeg.a
-	$(LN) $(LDFLAGS) -o jpegtran $(TROBJECTS) libjpeg.a $(LDLIBS)
+	$(LN) $(LDFLAGS) -o jpegtran$(AB_EXEEXT) $(TROBJECTS) libjpeg.a $(LDLIBS)
 
 rdjpgcom: rdjpgcom.o
-	$(LN) $(LDFLAGS) -o rdjpgcom rdjpgcom.o $(LDLIBS)
+	$(LN) $(LDFLAGS) -o rdjpgcom$(AB_EXEEXT) rdjpgcom.o $(LDLIBS)
 
 wrjpgcom: wrjpgcom.o
-	$(LN) $(LDFLAGS) -o wrjpgcom wrjpgcom.o $(LDLIBS)
+	$(LN) $(LDFLAGS) -o wrjpgcom$(AB_EXEEXT) wrjpgcom.o $(LDLIBS)
 
 jconfig.h: jconfig.txt
 	echo You must prepare a system-dependent jconfig.h file.
