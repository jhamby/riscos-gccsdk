--- src/ui/LineEditWidget.cpp
+++ src/ui/LineEditWidget.cpp
@@ -209,9 +209,9 @@ void LineEditWidget::initialize()
 {
 	setDragEnabled(true);
 
-	connect(this, &LineEditWidget::selectionChanged, this, &LineEditWidget::handleSelectionChanged);
-	connect(this, &LineEditWidget::textChanged, this, &LineEditWidget::handleTextChanged);
-	connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &LineEditWidget::notifyPasteActionStateChanged);
+	connect(this, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
+	connect(this, SIGNAL(textChanged(QString)), this, SLOT(handleTextChanged(QString)));
+	connect(QGuiApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(notifyPasteActionStateChanged()));
 }
 
 void LineEditWidget::resizeEvent(QResizeEvent *event)


