Index: libcpp/Makefile.in
===================================================================
--- libcpp/Makefile.in	(revision 170537)
+++ libcpp/Makefile.in	(working copy)
@@ -89,11 +89,11 @@
 
 libcpp_a_OBJS = charset.o directives.o directives-only.o errors.o \
 	expr.o files.o identifiers.o init.o lex.o line-map.o macro.o \
-	mkdeps.o pch.o symtab.o traditional.o
+	mkdeps.o pch.o rname.o symtab.o traditional.o
 
 libcpp_a_SOURCES = charset.c directives.c directives-only.c errors.c \
 	expr.c files.c identifiers.c init.c lex.c line-map.c macro.c \
-	mkdeps.c pch.c symtab.c traditional.c
+	mkdeps.c pch.c rname.c symtab.c traditional.c
 
 all: libcpp.a $(USED_CATALOGS)
 
