--- src/ui/ItemViewWidget.cpp
+++ src/ui/ItemViewWidget.cpp
@@ -264,9 +264,9 @@ ItemViewWidget::ItemViewWidget(QWidget *parent) : QTreeView(parent),
 
 	viewport()->setAcceptDrops(true);
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &ItemViewWidget::handleOptionChanged);
-	connect(m_headerWidget, &HeaderViewWidget::sortChanged, this, &ItemViewWidget::setSort);
-	connect(m_headerWidget, &HeaderViewWidget::columnVisibilityChanged, this, &ItemViewWidget::setColumnVisibility);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
+	connect(m_headerWidget, SIGNAL(sortChanged(int,Qt::SortOrder)), this, SLOT(setSort(int,Qt::SortOrder)));
+	connect(m_headerWidget, SIGNAL(columnVisibilityChanged(int,bool)), this, SLOT(setColumnVisibility(int,bool)));
 	connect(m_headerWidget, SIGNAL(sectionMoved(int,int,int)), this, SLOT(saveState()));
 }
 


