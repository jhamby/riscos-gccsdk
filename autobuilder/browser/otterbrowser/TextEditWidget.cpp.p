--- src/ui/TextEditWidget.cpp
+++ src/ui/TextEditWidget.cpp
@@ -44,9 +44,9 @@ TextEditWidget::TextEditWidget(QWidget *parent) : QPlainTextEdit(parent),
 
 void TextEditWidget::initialize()
 {
-	connect(this, &TextEditWidget::selectionChanged, this, &TextEditWidget::handleSelectionChanged);
-	connect(this, &TextEditWidget::textChanged, this, &TextEditWidget::handleTextChanged);
-	connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &TextEditWidget::notifyPasteActionStateChanged);
+	connect(this, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
+	connect(this, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
+	connect(QGuiApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(notifyPasteActionStateChanged()));
 }
 
 void TextEditWidget::focusInEvent(QFocusEvent *event)


