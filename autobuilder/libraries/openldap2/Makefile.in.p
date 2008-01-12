--- clients/tools/Makefile.in.orig	2008-01-11 18:49:57.000000000 -0800
+++ clients/tools/Makefile.in	2008-01-11 19:05:42.000000000 -0800
@@ -32,25 +32,25 @@
 
 
 ldapsearch:	ldsversion.o
-	$(LTLINK) -o $@ ldapsearch.o common.o ldsversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldapsearch.o common.o ldsversion.o $(LIBS)
 
 ldapmodify:	ldmversion.o
-	$(LTLINK) -o $@ ldapmodify.o common.o ldmversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldapmodify.o common.o ldmversion.o $(LIBS)
 
 ldapdelete:	lddversion.o
-	$(LTLINK) -o $@ ldapdelete.o common.o lddversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldapdelete.o common.o lddversion.o $(LIBS)
 
 ldapmodrdn:	ldrversion.o
-	$(LTLINK) -o $@ ldapmodrdn.o common.o ldrversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldapmodrdn.o common.o ldrversion.o $(LIBS)
 
 ldappasswd:	ldpversion.o
-	$(LTLINK) -o $@ ldappasswd.o common.o ldpversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldappasswd.o common.o ldpversion.o $(LIBS)
 
 ldapwhoami:	ldwversion.o
-	$(LTLINK) -o $@ ldapwhoami.o common.o ldwversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldapwhoami.o common.o ldwversion.o $(LIBS)
 
 ldapcompare: ldcversion.o
-	$(LTLINK) -o $@ ldapcompare.o common.o ldcversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldapcompare.o common.o ldcversion.o $(LIBS)
 
 ldsversion.c: Makefile
 	@-$(RM) $@
--- servers/slapd/Makefile.in.orig	2008-01-11 20:30:52.000000000 -0800
+++ servers/slapd/Makefile.in	2008-01-11 20:31:39.000000000 -0800
@@ -266,7 +266,7 @@
 	cp slapi/.libs/libslapi.a .
 
 slapd: $(SLAPD_DEPENDS) @LIBSLAPI@
-	$(LTLINK) -o $@ $(SLAPD_OBJECTS) $(LIBS) \
+	$(LTLINK) -o $@$(EXEEXT) $(SLAPD_OBJECTS) $(LIBS) \
 		$(WRAP_LIBS)
 	$(RM) $(SLAPTOOLS)
 	for i in $(SLAPTOOLS); do \
--- servers/slurpd/Makefile.in.orig	2008-01-11 21:14:19.000000000 -0800
+++ servers/slurpd/Makefile.in	2008-01-11 21:14:47.000000000 -0800
@@ -44,10 +44,10 @@
 XXXLIBS = $(LTHREAD_LIBS)
 
 slurpd:	version.o
-	$(LTLINK) -o $@ $(OBJS) version.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) $(OBJS) version.o $(LIBS)
 
 sslurpd:	version.o
-	$(LTLINK) -static -o $@ $(OBJS) version.o $(LIBS)
+	$(LTLINK) -static -o $@$(EXEEXT) $(OBJS) version.o $(LIBS)
 
 version.c: Makefile
 	@-$(RM) $@
