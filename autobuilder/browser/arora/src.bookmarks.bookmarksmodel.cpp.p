--- src/bookmarks/bookmarksmodel.cpp.orig	2015-04-10 20:47:29.368351907 +0100
+++ src/bookmarks/bookmarksmodel.cpp	2015-04-11 14:06:03.293981569 +0100
@@ -71,6 +71,7 @@
 
 #include <qbuffer.h>
 #include <qevent.h>
+#include <qmimedata.h>
 
 BookmarksModel::BookmarksModel(BookmarksManager *bookmarkManager, QObject *parent)
     : QAbstractItemModel(parent)
