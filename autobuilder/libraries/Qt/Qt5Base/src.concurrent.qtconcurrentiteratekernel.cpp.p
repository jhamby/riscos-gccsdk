--- src/concurrent/qtconcurrentiteratekernel.cpp.orig	2015-02-17 04:56:53.000000000 +0000
+++ src/concurrent/qtconcurrentiteratekernel.cpp	2015-04-12 13:43:51.640657032 +0100
@@ -31,6 +31,7 @@
 **
 ****************************************************************************/
 
+#include "qplatformdefs.h"
 #include "qtconcurrentiteratekernel.h"
 
 #if defined(Q_OS_MAC)
