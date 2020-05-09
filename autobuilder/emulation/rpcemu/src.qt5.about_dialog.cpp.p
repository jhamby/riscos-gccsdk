--- src/qt5/about_dialog.cpp.orig	2020-05-09 12:38:52.228396229 +0100
+++ src/qt5/about_dialog.cpp	2020-05-09 12:40:21.746616197 +0100
@@ -68,7 +68,7 @@
 	// Remove resize on Dialog
 	this->setFixedSize(this->sizeHint());
 
-	connect(buttons_box, &QDialogButtonBox::accepted, this, &QDialog::close);
+	connect(buttons_box, SIGNAL(accepted()), this, SLOT(close()));
 }
 
 AboutDialog::~AboutDialog()
