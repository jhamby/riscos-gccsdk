--- clients/tools/Makefile.in.orig	2009-01-21 16:00:42.000000000 -0800
+++ clients/tools/Makefile.in	2009-07-24 11:45:23.000000000 -0700
@@ -36,31 +36,31 @@
 
 
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
 
 ldapexop: ldeversion.o
-	$(LTLINK) -o $@ ldapexop.o common.o ldeversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldapexop.o common.o ldeversion.o $(LIBS)
 
 ldapurl: lduversion.o
-	$(LTLINK) -o $@ ldapurl.o lduversion.o $(LIBS)
+	$(LTLINK) -o $@$(EXEEXT) ldapurl.o lduversion.o $(LIBS)
 
 ldsversion.c: Makefile
 	@-$(RM) $@

--- servers/slapd/Makefile.in.orig	2009-06-02 15:36:17.000000000 -0700
+++ servers/slapd/Makefile.in	2009-07-24 12:11:59.000000000 -0700
@@ -266,7 +266,7 @@
 	cp slapi/.libs/libslapi.a .
 
 slapd: $(SLAPD_DEPENDS) @LIBSLAPI@
-	$(LTLINK) -o $@ $(SLAPD_OBJECTS) $(LIBS) \
+	$(LTLINK) -o $@$(EXEEXT) $(SLAPD_OBJECTS) $(LIBS) \
 		$(WRAP_LIBS)
 	$(RM) $(SLAPTOOLS)
 	for i in $(SLAPTOOLS); do \
