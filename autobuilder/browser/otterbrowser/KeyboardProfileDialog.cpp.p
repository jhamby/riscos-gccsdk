--- src/ui/preferences/KeyboardProfileDialog.cpp
+++ src/ui/preferences/KeyboardProfileDialog.cpp
@@ -44,13 +44,17 @@ ShortcutWidget::ShortcutWidget(const QKeySequence &shortcut, QWidget *parent) :
 		layout->setDirection(QBoxLayout::LeftToRight);
 		layout->addWidget(button);
 
-		connect(button, &QToolButton::clicked, this, &ShortcutWidget::clear);
+		connect(button, SIGNAL(clicked()), this, SLOT(clear()));
+#ifndef __riscos__
 		connect(this, &ShortcutWidget::keySequenceChanged, [=](const QKeySequence &shortcut)
 		{
 			button->setEnabled(!shortcut.isEmpty());
 
 			emit commitData(this);
 		});
+#else
+		connect(this, SIGNAL(keySequenceChanged(const QKeySequence)), this, SLOT(shortcutEnabled(const QKeySequence, QToolButton *button)));
+#endif
 	}
 }
 
@@ -238,6 +242,16 @@ KeyboardProfileDialog::~KeyboardProfileDialog()
 	delete m_ui;
 }
 
+#ifdef __riscos__
+void ShortcutWidget::shortcutEnabled(const QKeySequence &shortcut, QToolButton *button)
+{
+
+	button->setEnabled(!shortcut.isEmpty());
+
+	emit commitData(this);
+}
+#endif
+
 void KeyboardProfileDialog::changeEvent(QEvent *event)
 {
 	QDialog::changeEvent(event);


