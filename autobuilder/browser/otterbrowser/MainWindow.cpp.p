--- src/ui/MainWindow.cpp.orig	2016-04-23 10:20:06.967661869 +0100
+++ src/ui/MainWindow.cpp	2016-04-23 10:21:16.431661940 +0100
@@ -655,14 +655,14 @@
 				LocaleDialog dialog(this);
 				dialog.exec();
 			}
-
+#ifndef __riscos__
 			break;
 		case ActionsManager::CheckForUpdatesAction:
 			{
 				UpdateCheckerDialog *dialog(new UpdateCheckerDialog(this));
 				dialog->show();
 			}
-
+#endif
 			break;
 		case ActionsManager::DiagnosticReportAction:
 			{
