--- src/widgets/kernel/qtooltip.cpp.orig	2015-02-17 04:56:40.000000000 +0000
+++ src/widgets/kernel/qtooltip.cpp	2015-05-04 17:57:18.080055495 +0100
@@ -316,8 +316,9 @@
         break;
 
 
-#if defined (Q_OS_QNX) // On QNX the window activate and focus events are delayed and will appear
-                       // after the window is shown.
+#if defined (Q_OS_QNX) || defined (Q_OS_RISCOS)
+     // On QNX the window activate and focus events are delayed and will appear
+     // after the window is shown.
     case QEvent::WindowActivate:
     case QEvent::FocusIn:
         return false;
