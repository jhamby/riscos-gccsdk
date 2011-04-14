--- libcpp/Makefile.in.orig	2006-10-11 23:44:17.000000000 +0200
+++ libcpp/Makefile.in	2006-10-11 23:43:59.000000000 +0200
@@ -71,12 +71,12 @@
 
 libcpp_a_OBJS = charset.o directives.o errors.o expr.o files.o \
 	identifiers.o init.o lex.o line-map.o macro.o mkdeps.o \
-	pch.o symtab.o traditional.o
+	pch.o symtab.o traditional.o rname.o
 makedepend_OBJS = makedepend.o
 
 libcpp_a_SOURCES = charset.c directives.c errors.c expr.c files.c \
 	identifiers.c init.c lex.c line-map.c macro.c mkdeps.c \
-	pch.c symtab.c traditional.c
+	pch.c symtab.c traditional.c rname.c
 
 all: libcpp.a makedepend$(EXEEXT) $(USED_CATALOGS)
 
