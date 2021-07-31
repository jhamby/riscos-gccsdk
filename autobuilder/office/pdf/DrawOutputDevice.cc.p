--- pdf/DrawOutputDevice.cc.orig	2017-03-04 10:07:41.794820019 +0000
+++ pdf/DrawOutputDevice.cc	2017-03-04 10:12:29.657110263 +0000
@@ -441,6 +441,8 @@
     }
 
     int x,y;
+    if (clipbox.getPath()){
+
     for (const int* p=clipbox.getPath()+2;*p!=0;p++)
     {
       switch (*p)
@@ -469,6 +471,8 @@
       }
     }
 
+    }
+
     return 1;
   }
   return 0;
