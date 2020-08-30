--- src/core/Migrator.cpp
+++ src/core/Migrator.cpp
@@ -574,12 +574,19 @@ bool Migrator::run()
 
 	QDialogButtonBox::StandardButton clickedButton(QDialogButtonBox::Yes);
 
+#ifndef __riscos__
 	QObject::connect(buttonBox, &QDialogButtonBox::clicked, [&](QAbstractButton *button)
 	{
 		clickedButton = buttonBox->standardButton(button);
 
 		dialog.close();
 	});
+#else
+//	QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), buttonBox, SLOT(buttonClicked(QAbstractButton*,
+//		QDialogButtonBox*, QDialogButtonBox::StandardButton, QDialog)));
+	QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), buttonBox, SLOT(buttonClicked()));
+		
+#endif
 
 	dialog.layout()->addWidget(label);
 	dialog.layout()->addWidget(migrationsViewWidget);
@@ -620,4 +627,25 @@ bool Migrator::run()
 	return true;
 }
 
+#ifdef __riscos__
+
+
+/* void Migrator::buttonClicked(QAbstractButton *button, QDialogButtonBox *buttonBox, QDialogButtonBox::StandardButton clickedButton, QDialog dialog)
+{
+		clickedButton = buttonBox->standardButton(button);
+
+		dialog.close();
+
+} */
+
+
+/* void Migrator::buttonClicked()
+{
+		clickedButton = buttonBox->standardButton(button);
+
+		dialog.close();
+
+} */
+#endif
+
 }


