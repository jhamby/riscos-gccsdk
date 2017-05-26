--- src/config.cpp.orig	2005-09-14 05:24:34.000000000 +0100
+++ src/config.cpp	2017-05-24 12:58:48.689010000 +0100
@@ -74,8 +74,9 @@
         cvp.userData = &errorMessage;
         cvp.error = &accumError;
 
+// Error changed to not fatal due to a regression in libxml2
         if (!xmlValidateDocument(&cvp, doc))
-            errorFatal("xml parse error:\n%s", errorMessage.c_str());        
+            errorWarning("xml parse error:\n%s", errorMessage.c_str());        
     }
 }
 
