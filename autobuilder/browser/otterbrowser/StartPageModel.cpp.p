--- src/modules/windows/web/StartPageModel.cpp
+++ src/modules/windows/web/StartPageModel.cpp
@@ -38,13 +38,13 @@ StartPageModel::StartPageModel(QObject *parent) : QStandardItemModel(parent),
 	handleOptionChanged(SettingsManager::Backends_WebOption);
 	reloadModel();
 
-	connect(BookmarksManager::getModel(), &BookmarksModel::bookmarkAdded, this, &StartPageModel::handleBookmarkModified);
-	connect(BookmarksManager::getModel(), &BookmarksModel::bookmarkModified, this, &StartPageModel::handleBookmarkModified);
-	connect(BookmarksManager::getModel(), &BookmarksModel::bookmarkRestored, this, &StartPageModel::handleBookmarkModified);
-	connect(BookmarksManager::getModel(), &BookmarksModel::bookmarkMoved, this, &StartPageModel::handleBookmarkMoved);
-	connect(BookmarksManager::getModel(), &BookmarksModel::bookmarkTrashed, this, &StartPageModel::handleBookmarkMoved);
-	connect(BookmarksManager::getModel(), &BookmarksModel::bookmarkRemoved, this, &StartPageModel::handleBookmarkRemoved);
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &StartPageModel::handleOptionChanged);
+	connect(BookmarksManager::getModel(), SIGNAL(bookmarkAdded(BookmarksItem*)), this, SLOT(handleBookmarkModified(BookmarksItem*)));
+	connect(BookmarksManager::getModel(), SIGNAL(bookmarkModified(BookmarksItem*)), this, SLOT(handleBookmarkModified(BookmarksItem*)));
+	connect(BookmarksManager::getModel(), SIGNAL(bookmarkRestored(BookmarksItem*)), this, SLOT(handleBookmarkModified(BookmarksItem*)));
+	connect(BookmarksManager::getModel(), SIGNAL(bookmarkMoved(BookmarksItem*,BookmarksItem*,int)), this, SLOT(handleBookmarkMoved(BookmarksItem*,BookmarksItem*)));
+	connect(BookmarksManager::getModel(), SIGNAL(bookmarkTrashed(BookmarksItem*,BookmarksItem*)), this, SLOT(handleBookmarkMoved(BookmarksItem*,BookmarksItem*)));
+	connect(BookmarksManager::getModel(), SIGNAL(bookmarkRemoved(BookmarksItem*,BookmarksItem*)), this, SLOT(handleBookmarkRemoved(BookmarksItem*,BookmarksItem*)));
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
 }
 
 void StartPageModel::dragEnded()


