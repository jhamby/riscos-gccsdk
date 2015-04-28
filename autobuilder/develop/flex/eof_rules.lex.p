--- examples/manual/eof_rules.lex.orig	2012-12-07 04:00:17.000000000 +1300
+++ examples/manual/eof_rules.lex	2015-04-23 20:43:54.684623000 +1200
@@ -37,8 +37,7 @@
 
           BEGIN(INITIAL);
         }
-<INCLUDE><<EOF>> 
-        {
+<INCLUDE><<EOF>> {
             fprintf( stderr, "EOF in include" );
             yyterminate();
         }
