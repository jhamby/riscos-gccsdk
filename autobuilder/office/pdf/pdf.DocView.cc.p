--- pdf/DocView.cc.orig	2021-08-11 12:20:11.789617703 -0700
+++ pdf/DocView.cc	2021-08-11 12:21:50.218392575 -0700
@@ -599,7 +599,7 @@
            }
 
            if (adjust)
-              makeDocView(new_doc,choices,page);
+              makeDocView(new_doc,choices,page,0);
            else
               set(*new_doc,choices,page);
 
@@ -628,7 +628,7 @@
            if (!new_doc) break;
 
            if (adjust)
-              makeDocView(new_doc,choices,1);
+              makeDocView(new_doc,choices,1,0);
            else
               set(*new_doc,choices,1);
            break;
@@ -666,7 +666,7 @@
            if (page)
            {
              if (adjust)
-                 makeDocView(document,choices,page);
+                 makeDocView(document,choices,page,0);
              else
                  set(*document,choices,page);
            }
