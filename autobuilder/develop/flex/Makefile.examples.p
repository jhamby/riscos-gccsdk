--- examples/manual/Makefile.examples.orig	2012-12-07 04:00:17.000000000 +1300
+++ examples/manual/Makefile.examples	2015-04-23 21:08:21.556568000 +1200
@@ -8,7 +8,8 @@
 #
 #############################################################
 
-CC       = gcc -g
+SHELL    = /bin/sh
+CC       = gcc -static -LLibFL:
 LEX      = flex -i -I 
 YACC     = bison -d -y
 ALLOCA   =
@@ -34,7 +35,7 @@
 
 numbers: numbers.lex
 	$(LEX)  numbers.lex
-	$(CC)   lex.yy.c -o numbers
+	$(CC)   lex.yy.c -o numbers -ll
 
 dates: dates.lex
 	$(LEX)  dates.lex
@@ -81,8 +82,7 @@
 	$(CC) -o user_act lex.yy.c -ll 
 
 clean:
-	rm -f *.BAK *.o core *~* *.a 
-	rm -f *.tab.h *.tab.c
-	rm -f myname expr lex.yy.c *.out eof wc yymore
+	rm -f y.tab.h y.tab.c
+	rm -f myname expr lex.yy.c eof wc yymore
 	rm -f replace front user_act string1 string2
 	rm -f dates numbers cat
