--- src/tools/qlalr/recognizer.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qlalr/recognizer.cpp	2014-08-17 15:08:25.019849117 +0100
@@ -89,7 +89,7 @@
   if (ch.isNull ())
     return EOF_SYMBOL;
 
-  int token = ch.unicode ();
+  int token = RISCOS_DEREF_QCHAR(ch.unicode ());
 
   if (token == '"')
     {
@@ -199,7 +199,7 @@
         {
           while (! ch.isNull ())
             {
-              token = ch.unicode ();
+              token = RISCOS_DEREF_QCHAR(ch.unicode ());
               inp ();
 
               if (token == ':' && ch == QLatin1Char ('/'))
@@ -237,7 +237,7 @@
         {
           while (! ch.isNull ())
             {
-              token = ch.unicode ();
+              token = RISCOS_DEREF_QCHAR(ch.unicode ());
               inp ();
 
               if (token == '.' && ch == QLatin1Char ('/'))
