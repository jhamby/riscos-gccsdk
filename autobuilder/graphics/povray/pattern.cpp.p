--- source/pattern.cpp.orig	2004-08-03 00:11:36.000000000 +0100
+++ source/pattern.cpp	2011-11-27 15:00:07.000000000 +0000
@@ -788,7 +788,7 @@
   }
   else
   {
-    noise = min(1.0, noise);
+    noise = min(static_cast<double>(1.0), noise);
     noise = pow(noise, 0.77);
   }
 
@@ -1046,7 +1046,7 @@
 
   POV_SRAND(temp);  /* restore */
 
-  return min(tf, 1.0);
+  return min(tf, static_cast<double>(1.0));
 }
 
 
@@ -1399,7 +1399,7 @@
       TPat->Vals.Crackle.Form[Z]*pow(minsum3, 1.0/Metric); 
   }
 
-  return max(min(tf, 1.), 0.);
+  return max(min(tf, static_cast<double>(1.)), static_cast<double>(0.));
 }
 
 
@@ -3860,7 +3860,7 @@
   if(noise_generator>1)
   {
     noise = Noise(EPoint, TPat)*2.0-0.5;
-    value = min(max(noise,0.0),1.0);
+    value = min(max(noise,static_cast<double>(0.0)),static_cast<double>(1.0));
   }
   else
   {
@@ -3874,7 +3874,7 @@
     if(noise_generator>1)
     {
       noise = Noise(temp, TPat)*2.0-0.5;
-      value += omega * min(max(noise,0.0),1.0);
+      value += omega * min(max(noise,static_cast<double>(0.0)),static_cast<double>(1.0));
     }
     else
     {
