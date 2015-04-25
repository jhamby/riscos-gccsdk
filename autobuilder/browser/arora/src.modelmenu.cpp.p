--- src/modelmenu.cpp.orig	2015-04-10 20:47:29.380351907 +0100
+++ src/modelmenu.cpp	2015-04-11 14:02:48.405979043 +0100
@@ -67,6 +67,8 @@
 
 #include <qabstractitemmodel.h>
 #include <qapplication.h>
+#include <qdrag.h>
+#include <qmimedata.h>
 #include <qevent.h>
 
 #include <qdebug.h>
@@ -344,7 +346,7 @@
     if (drag->exec() == Qt::MoveAction) {
         m_model->removeRow(idx.row(), m_root);
 
-        if (!this->isAncestorOf(drag->target()))
+        if (!this->isAncestorOf(static_cast<QWidget*>(drag->target())))
             close();
         else
             aboutToShow();
