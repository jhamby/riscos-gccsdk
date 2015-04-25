--- src/autofilldialog.cpp.orig	2015-04-10 20:47:29.368351907 +0100
+++ src/autofilldialog.cpp	2015-04-11 13:51:53.129970552 +0100
@@ -46,7 +46,8 @@
 {
     AutoFillManager *manager = BrowserApplication::instance()->autoFillManager();
     m_forms = manager->forms();
-    reset();
+    beginResetModel();
+    endResetModel();
 }
 
 QVariant AutoFillModel::headerData(int section, Qt::Orientation orientation, int role) const
