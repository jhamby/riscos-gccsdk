--- src/corelib/kernel/qsharedmemory.h.orig	2015-02-17 04:56:50.000000000 +0000
+++ src/corelib/kernel/qsharedmemory.h	2015-04-12 13:43:51.552657030 +0100
@@ -34,6 +34,7 @@
 #ifndef QSHAREDMEMORY_H
 #define QSHAREDMEMORY_H
 
+#include "qplatformdefs.h"
 #include <QtCore/qobject.h>
 
 QT_BEGIN_NAMESPACE
