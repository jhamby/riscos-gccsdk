--- Source/JavaScriptCore/runtime/Options.h.orig	2014-05-15 18:12:35.000000000 +0100
+++ Source/JavaScriptCore/runtime/Options.h	2014-06-25 19:43:40.488666505 +0100
@@ -168,8 +168,12 @@
     \
     v(unsigned, gcMaxHeapSize, 0) \
     v(bool, recordGCPauseTimes, false) \
-    v(bool, logHeapStatisticsAtExit, false) 
-
+    v(bool, logHeapStatisticsAtExit, false) \
+    \
+    v(bool, useHalfWord, false) \
+    v(bool, useDoubleWord, false) \
+    v(bool, useBX, false)
+    
 class Options {
 public:
     // This typedef is to allow us to eliminate the '_' in the field name in
