--- src/corelib/kernel/qsharedmemory.h.orig	2013-08-25 19:03:38.000000000 +0100
+++ src/corelib/kernel/qsharedmemory.h	2013-11-14 11:53:10.454189422 +0000
@@ -42,6 +42,7 @@
 #ifndef QSHAREDMEMORY_H
 #define QSHAREDMEMORY_H
 
+#include "qplatformdefs.h"
 #include <QtCore/qobject.h>
 
 QT_BEGIN_NAMESPACE
