--- src/concurrent/qtconcurrentiteratekernel.cpp.orig	2013-08-25 19:03:31.000000000 +0100
+++ src/concurrent/qtconcurrentiteratekernel.cpp	2013-11-14 09:54:36.558097237 +0000
@@ -39,6 +39,7 @@
 **
 ****************************************************************************/
 
+#include "qplatformdefs.h"
 #include "qtconcurrentiteratekernel.h"
 
 #if defined(Q_OS_MAC)
