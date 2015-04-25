--- src/locationbar/locationbarsiteicon.cpp.orig	2015-04-10 20:47:29.380351907 +0100
+++ src/locationbar/locationbarsiteicon.cpp	2015-04-11 14:08:39.073983588 +0100
@@ -19,7 +19,9 @@
 
 #include "locationbarsiteicon.h"
 
+#include <qdrag.h>
 #include <qevent.h>
+#include <qmimedata.h>
 #include <qurl.h>
 
 #include "browserapplication.h"
