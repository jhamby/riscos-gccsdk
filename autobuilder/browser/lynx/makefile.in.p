--- src/chrtrans/makefile.in.old	2004-02-15 20:17:04.000000000 +0000
+++ src/chrtrans/makefile.in	2004-02-15 20:09:58.000000000 +0000
@@ -15,7 +15,7 @@
 top_builddir	= ../..
 
 LIBS		= @LIBS@ $(SITE_LIBS)
-LDFLAGS		= @LDFLAGS@
+LDFLAGS		= 
 
 INTLLIB		= @INTLDIR_MAKE@@INTLLIBS@
 
@@ -97,7 +97,7 @@
 tables: $(TABLES)
 
 makeuctb$x: makeuctb$o
-	$(CC) $(CC_OPTS) $(LDFLAGS) -o $@ makeuctb$o $(INTLLIB) $(LIBS)
+	gcc $(CC_OPTS) $(LDFLAGS) -o $@ makeuctb$o $(INTLLIB) $(LIBS)
 
 makeuctb$o: $(srcdir)/UCkd.h $(srcdir)/makeuctb.c
 
@@ -105,7 +105,7 @@
 
 .c$o:
 @RULE_CC@
-	@ECHO_CC@$(CC) $(CC_OPTS) -c $(srcdir)/$*.c
+	@ECHO_CC@gcc $(CC_OPTS) -c $(srcdir)/$*.c
 
 .c.i:
 @RULE_CC@
