--- src/Makefile.in.orig	2018-02-21 13:48:28.733318999 +1300
+++ src/Makefile.in	2018-03-02 21:04:23.892566737 +1300
@@ -63,7 +63,7 @@
 am__installdirs = "$(DESTDIR)$(bindir)" "$(DESTDIR)$(man1dir)"
 PROGRAMS = $(bin_PROGRAMS)
 am__objects_1 = alias.$(OBJEXT) arith_yacc.$(OBJEXT) \
-	arith_yylex.$(OBJEXT) cd.$(OBJEXT) error.$(OBJEXT) \
+	arith_yylex.$(OBJEXT) cat.$(OBJEXT) cd.$(OBJEXT) error.$(OBJEXT) \
 	eval.$(OBJEXT) exec.$(OBJEXT) expand.$(OBJEXT) \
 	histedit.$(OBJEXT) input.$(OBJEXT) jobs.$(OBJEXT) \
 	mail.$(OBJEXT) main.$(OBJEXT) memalloc.$(OBJEXT) \
@@ -216,7 +216,7 @@
 COMMON_CPPFLAGS = \
 	-include $(top_builddir)/config.h \
 	-DBSD=1 -DSHELL \
-	-DIFS_BROKEN
+	-DIFS_BROKEN -DJOBS=0
 
 AM_CFLAGS = $(COMMON_CFLAGS)
 AM_CPPFLAGS = $(COMMON_CPPFLAGS)
@@ -231,7 +231,7 @@
 	alias.c arith_yacc.c arith_yylex.c cd.c error.c eval.c exec.c expand.c \
 	histedit.c input.c jobs.c mail.c main.c memalloc.c miscbltin.c \
 	mystring.c options.c parser.c redir.c show.c trap.c output.c \
-	bltin/printf.c system.c bltin/test.c bltin/times.c var.c
+	bltin/cat.c bltin/printf.c system.c bltin/test.c bltin/times.c var.c
 
 dash_SOURCES = \
 	$(dash_CFILES) \
@@ -342,6 +342,7 @@
 @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/alias.Po@am__quote@
 @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/arith_yacc.Po@am__quote@
 @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/arith_yylex.Po@am__quote@
+@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/cat.Po@am__quote@
 @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/cd.Po@am__quote@
 @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/error.Po@am__quote@
 @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/eval.Po@am__quote@
@@ -395,6 +396,20 @@
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
 @am__fastdepCC_FALSE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o printf.obj `if test -f 'bltin/printf.c'; then $(CYGPATH_W) 'bltin/printf.c'; else $(CYGPATH_W) '$(srcdir)/bltin/printf.c'; fi`
 
+cat.o: bltin/cat.c
+@am__fastdepCC_TRUE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -MT cat.o -MD -MP -MF $(DEPDIR)/cat.Tpo -c -o cat.o `test -f 'bltin/cat.c' || echo '$(srcdir)/'`bltin/cat.c
+@am__fastdepCC_TRUE@	$(am__mv) $(DEPDIR)/cat.Tpo $(DEPDIR)/cat.Po
+@AMDEP_TRUE@@am__fastdepCC_FALSE@	source='bltin/cat.c' object='cat.o' libtool=no @AMDEPBACKSLASH@
+@AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
+@am__fastdepCC_FALSE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o cat.o `test -f 'bltin/cat.c' || echo '$(srcdir)/'`bltin/cat.c
+
+cat.obj: bltin/cat.c
+@am__fastdepCC_TRUE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -MT cat.obj -MD -MP -MF $(DEPDIR)/cat.Tpo -c -o cat.obj `if test -f 'bltin/cat.c'; then $(CYGPATH_W) 'bltin/cat.c'; else $(CYGPATH_W) '$(srcdir)/bltin/cat.c'; fi`
+@am__fastdepCC_TRUE@	$(am__mv) $(DEPDIR)/cat.Tpo $(DEPDIR)/cat.Po
+@AMDEP_TRUE@@am__fastdepCC_FALSE@	source='bltin/cat.c' object='cat.obj' libtool=no @AMDEPBACKSLASH@
+@AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
+@am__fastdepCC_FALSE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -c -o cat.obj `if test -f 'bltin/cat.c'; then $(CYGPATH_W) 'bltin/cat.c'; else $(CYGPATH_W) '$(srcdir)/bltin/cat.c'; fi`
+
 test.o: bltin/test.c
 @am__fastdepCC_TRUE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) -MT test.o -MD -MP -MF $(DEPDIR)/test.Tpo -c -o test.o `test -f 'bltin/test.c' || echo '$(srcdir)/'`bltin/test.c
 @am__fastdepCC_TRUE@	$(am__mv) $(DEPDIR)/test.Tpo $(DEPDIR)/test.Po
@@ -708,8 +723,8 @@
 syntax.c syntax.h: mksyntax
 	./$^
 
-signames.c: mksignames
-	./$^
+signames.c:
+	echo error: pre-install RISC OS native signames.c
 
 mksyntax: token.h
 
