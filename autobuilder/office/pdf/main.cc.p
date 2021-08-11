--- pdf/main.cc.orig	2021-08-11 12:25:05.399916672 -0700
+++ pdf/main.cc	2021-08-11 12:25:34.244140776 -0700
@@ -64,9 +64,9 @@
 
 // first part is the xpdf version number
 #ifndef __APCS_32
-#define  VERSION "3.02.1.24"
+#define  VERSION "3.02.1.24.1"
 #else
-#define  VERSION "3.02.1.24"
+#define  VERSION "3.02.1.24.1"
 #endif
 
 class Application : public Node
@@ -125,7 +125,7 @@
 bool Application::createDocument(const char* filename)
 {
    DocViewChoices choices;
-   return makeDocView(makeDocument(*this,filename),choices);
+   return makeDocView(makeDocument(*this,filename),choices,1,0);
 }
 
 //*************************************************************************
