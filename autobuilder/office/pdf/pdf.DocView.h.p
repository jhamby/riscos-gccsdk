--- pdf/DocView.h.orig	2021-08-11 12:13:54.458585853 -0700
+++ pdf/DocView.h	2021-08-11 12:17:26.260301786 -0700
@@ -84,7 +84,7 @@
 class DocView : public Node
 {
   public:
-    friend bool       makeDocView(Document*,const DocViewChoices&,int page=1,int logicalPageOffset=0);
+    friend bool       makeDocView(Document*,const DocViewChoices&,int page,int logicalPageOffset);
     void              showPage();
     int               getPage()               {return toolbar.getPage();}
     void              setPage(const DocPage&,bool add_to_history=1);
