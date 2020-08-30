--- src/ui/preferences/KeyboardProfileDialog.h
+++ src/ui/preferences/KeyboardProfileDialog.h
@@ -26,6 +26,7 @@
 
 #include <QtCore/QModelIndex>
 #include <QtWidgets/QKeySequenceEdit>
+#include <QtWidgets/QToolButton>
 
 namespace Otter
 {
@@ -46,6 +47,10 @@ public:
 
 signals:
 	void commitData(QWidget *editor);
+
+protected slots:
+	void shortcutEnabled(const QKeySequence &shortcut, QToolButton *button);
+
 };
 
 class KeyboardActionDelegate final : public ItemDelegate


