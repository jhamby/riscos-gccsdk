--- src/core/Migrator.h
+++ src/core/Migrator.h
@@ -21,6 +21,8 @@
 #define OTTER_MIGRATOR_H
 
 #include <QtCore/QString>
+#include <QAbstractButton>
+#include <QtWidgets/QDialogButtonBox>
 
 namespace Otter
 {
@@ -39,6 +41,7 @@ public:
 
 protected:
 	static QString createBackupPath(const QString &sourcePath);
+
 };
 
 class Migrator final
@@ -46,6 +49,10 @@ class Migrator final
 
 public:
 	static bool run();
+
+//protected slots:
+//	void buttonClicked(QAbstractButton *button, QDialogButtonBox *buttonBox, QDialogButtonBox::StandardButton clickedButton, QDialog dialog);
+//	void buttonClicked();
 };
 
 }


