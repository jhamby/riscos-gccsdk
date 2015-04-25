--- src/adblock/adblockmodel.cpp.orig	2015-04-10 20:47:29.364351907 +0100
+++ src/adblock/adblockmodel.cpp	2015-04-11 14:05:33.701981185 +0100
@@ -41,7 +41,8 @@
 
 void AdBlockModel::rulesChanged()
 {
-    reset();
+    beginResetModel();
+    endResetModel();
 }
 
 const AdBlockRule AdBlockModel::rule(const QModelIndex &index) const
@@ -67,7 +68,7 @@
     int row = m_manager->subscriptions().indexOf(subscription);
     if (row < 0 || row >= m_manager->subscriptions().count())
         return QModelIndex();
-    return createIndex(row, 0, 0);
+    return createIndex(row, 0);
 }
 
 QVariant AdBlockModel::headerData(int section, Qt::Orientation orientation, int role) const
@@ -142,7 +143,7 @@
         return QModelIndex();
 
     if (!parent.isValid())
-        return createIndex(row, column, (void*)0);
+        return createIndex(row, column);
 
     // get the parent node
     const AdBlockSubscription *parentNode = subscription(parent);
@@ -159,7 +160,7 @@
         return QModelIndex();
 
     int parentRow = m_manager->subscriptions().indexOf(parent);
-    return createIndex(parentRow, 0, 0);
+    return createIndex(parentRow, 0);
 }
 
 Qt::ItemFlags AdBlockModel::flags(const QModelIndex &index) const
