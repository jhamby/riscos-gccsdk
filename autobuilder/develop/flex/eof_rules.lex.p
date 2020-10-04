--- examples/manual/eof_rules.lex.orig	2016-10-21 13:43:44.000000000 +1300
+++ examples/manual/eof_rules.lex	2020-09-05 22:01:02.770159132 +1200
@@ -37,8 +37,7 @@
 
           BEGIN(INITIAL);
         }
-<INCLUDE><<EOF>> 
-        {
+<INCLUDE><<EOF>> {
             fprintf( stderr, "EOF in include" );
             yyterminate();
         }
