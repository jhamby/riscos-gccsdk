--- src/history/history.cpp.orig	2010-09-27 03:42:17.000000000 +0100
+++ src/history/history.cpp	2015-04-23 20:53:07.264163475 +0100
@@ -104,7 +104,8 @@
 
 void HistoryModel::historyReset()
 {
-    reset();
+    beginResetModel();
+    endResetModel();
 }
 
 void HistoryModel::entryAdded()
@@ -535,7 +536,8 @@
 void HistoryFilterModel::sourceReset()
 {
     m_loaded = false;
-    reset();
+    beginResetModel();
+    endResetModel();
 }
 
 int HistoryFilterModel::rowCount(const QModelIndex &parent) const
@@ -670,8 +672,10 @@
     connect(sourceModel(), SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
             this, SLOT(sourceRowsRemoved(const QModelIndex &, int, int)));
     m_loaded = false;
-    if (oldCount - count != rowCount())
-        reset();
+    if (oldCount - count != rowCount()) {
+        beginResetModel();
+        endResetModel();
+    }
     return true;
 }
 
@@ -815,7 +819,7 @@
         return QModelIndex();
 
     if (!parent.isValid())
-        return createIndex(row, column, 0);
+        return createIndex(row, column);
     return createIndex(row, column, parent.row() + 1);
 }
 
@@ -824,7 +828,7 @@
     int offset = index.internalId();
     if (offset == 0 || !index.isValid())
         return QModelIndex();
-    return createIndex(offset - 1, 0, 0);
+    return createIndex(offset - 1, 0);
 }
 
 bool HistoryTreeModel::hasChildren(const QModelIndex &parent) const
@@ -864,13 +868,15 @@
                 this, SLOT(sourceRowsRemoved(const QModelIndex &, int, int)));
     }
 
-    reset();
+    beginResetModel();
+    endResetModel();
 }
 
 void HistoryTreeModel::sourceReset()
 {
     m_sourceRowCache.clear();
-    reset();
+    beginResetModel();
+    endResetModel();
 }
 
 void HistoryTreeModel::sourceRowsInserted(const QModelIndex &parent, int start, int end)
@@ -878,8 +884,7 @@
     Q_UNUSED(parent); // Avoid warnings when compiling release
     Q_ASSERT(!parent.isValid());
     if (start != 0 || start != end) {
-        m_sourceRowCache.clear();
-        reset();
+        sourceReset();
         return;
     }
 
@@ -941,8 +946,7 @@
 void HistoryTreeModel::sourceRowsRemoved(const QModelIndex &parent, int start, int end)
 {
     if (!removingDown) {
-        reset();
-        m_sourceRowCache.clear();
+        sourceReset();
         return;
     }
     Q_UNUSED(parent); // Avoid warnings when compiling release
