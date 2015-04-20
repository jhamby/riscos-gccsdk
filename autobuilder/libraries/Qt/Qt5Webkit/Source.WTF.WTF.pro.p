--- Source/WTF/WTF.pro.orig	2015-02-17 04:57:01.000000000 +0000
+++ Source/WTF/WTF.pro	2015-04-20 14:24:34.271641782 +0100
@@ -278,3 +278,8 @@
     # For GCC 4.5 and before we disable C++0x mode in JSC for if enabled in Qt's mkspec
     QMAKE_CXXFLAGS -= -std=c++0x -std=gnu++0x -std=c++11 -std=gnu++11
 }
+ 
+arm*-riscos*-* {
+    SOURCES -= OSAllocatorPosix.cpp
+    SOURCES += ./OSAllocatorRiscos.cpp ./RiscosBrkpntHandler.cpp
+}
