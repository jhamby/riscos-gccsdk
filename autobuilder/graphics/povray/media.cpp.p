--- source/media.cpp.orig	2004-08-03 00:11:36.000000000 +0100
+++ source/media.cpp	2011-11-27 15:00:07.000000000 +0000
@@ -1427,7 +1427,7 @@
     {
       /* Insert light source intersections into light list. */
 
-      t1 = max(t1, 0.0);
+      t1 = max(t1, static_cast<double>(0.0));
       t2 = min(t2, Inter->Depth);
 
       light->active = true;
