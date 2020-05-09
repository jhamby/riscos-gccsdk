--- src/qt5/configure_dialog.cpp.orig	2020-05-09 12:17:07.022383194 +0100
+++ src/qt5/configure_dialog.cpp	2020-05-09 12:40:21.746616197 +0100
@@ -130,13 +130,13 @@
 	grid->addWidget(buttons_box, 3, 0, 1, 2);       // span 2 columns
 
 	// Connect actions to widgets
-	connect(refresh_slider, &QSlider::valueChanged, this, &ConfigureDialog::slider_moved);
+	connect(refresh_slider, SIGNAL(valueChanged(int)), this, SLOT(slider_moved(int)));
 
-	connect(buttons_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
-	connect(buttons_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
+	connect(buttons_box, SIGNAL(accepted()), this, SLOT(accept()));
+	connect(buttons_box, SIGNAL(rejected()), this, SLOT(reject()));
 
-	connect(this, &QDialog::accepted, this, &ConfigureDialog::dialog_accepted);
-	connect(this, &QDialog::rejected, this, &ConfigureDialog::dialog_rejected);
+	connect(this, SIGNAL(accepted()), this, SLOT(dialog_accepted()));
+	connect(this, SIGNAL(rejected()), this, SLOT(dialog_rejected()));
 
 	// Set the values in the configure dialog box
 	applyConfig();
