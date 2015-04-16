--- examples/widgets/widgets/wiggly/dialog.cpp.orig	2015-04-16 12:01:06.000000000 +0100
+++ examples/widgets/widgets/wiggly/dialog.cpp	2015-04-16 12:01:21.413436507 +0100
@@ -55,7 +55,8 @@
     layout->addWidget(wigglyWidget);
     layout->addWidget(lineEdit);
 
-    connect(lineEdit, &QLineEdit::textChanged, wigglyWidget, &WigglyWidget::setText);
+    connect(lineEdit, SIGNAL(textChanged(QString)),
+	    wigglyWidget, SLOT(setText(QString)));
     lineEdit->setText(tr("Hello world!"));
 
     setWindowTitle(tr("Wiggly"));
