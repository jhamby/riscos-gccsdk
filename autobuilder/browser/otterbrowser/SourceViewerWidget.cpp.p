--- src/ui/SourceViewerWidget.cpp
+++ src/ui/SourceViewerWidget.cpp
@@ -345,9 +345,9 @@ SourceViewerWidget::SourceViewerWidget(QWidget *parent) : QPlainTextEdit(parent)
 	handleOptionChanged(SettingsManager::SourceViewer_ShowLineNumbersOption, SettingsManager::getOption(SettingsManager::SourceViewer_ShowLineNumbersOption));
 	handleOptionChanged(SettingsManager::SourceViewer_WrapLinesOption, SettingsManager::getOption(SettingsManager::SourceViewer_WrapLinesOption));
 
-	connect(this, &SourceViewerWidget::textChanged, this, &SourceViewerWidget::updateSelection);
-	connect(this, &SourceViewerWidget::cursorPositionChanged, this, &SourceViewerWidget::updateTextCursor);
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &SourceViewerWidget::handleOptionChanged);
+	connect(this, SIGNAL(textChanged()), this, SLOT(updateSelection()));
+	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateTextCursor()));
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int, const QVariant)), this, SLOT(handleOptionChanged(int, const QVariant)));
 }
 
 void SourceViewerWidget::resizeEvent(QResizeEvent *event)


