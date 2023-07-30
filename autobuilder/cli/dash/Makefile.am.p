--- src/Makefile.am.orig	2023-07-30 14:29:55.291782632 -0700
+++ src/Makefile.am	2023-07-30 14:41:39.155362247 -0700
@@ -2,7 +2,7 @@
 
 COMMON_CFLAGS = -Wall
 COMMON_CPPFLAGS = \
-	-DBSD=1 -DSHELL
+	-DBSD=1 -DSHELL -DJOBS=0
 
 AM_CFLAGS = $(COMMON_CFLAGS)
 AM_CPPFLAGS = -include $(top_builddir)/config.h $(COMMON_CPPFLAGS)
@@ -21,6 +21,7 @@
 	alias.c arith_yacc.c arith_yylex.c cd.c error.c eval.c exec.c expand.c \
 	histedit.c input.c jobs.c mail.c main.c memalloc.c miscbltin.c \
 	mystring.c options.c parser.c priv.c redir.c show.c trap.c output.c \
+	bltin/cat.c bltin/ls.c bltin/mv.c bltin/rm.c \
 	bltin/printf.c system.c bltin/test.c bltin/times.c var.c
 dash_SOURCES = \
 	$(dash_CFILES) \
@@ -62,8 +63,8 @@
 syntax.c syntax.h: mksyntax
 	$(AM_V_GEN)./$^
 
-signames.c: mksignames
-	$(AM_V_GEN)./$^
+signames.c:
+	echo error: pre-install RISC OS native signames.c
 
 mksyntax: token.h
 
