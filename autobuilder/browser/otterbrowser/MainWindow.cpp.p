--- src/ui/MainWindow.cpp.orig	2015-10-31 12:48:27.202851371 +0000
+++ src/ui/MainWindow.cpp	2015-10-31 12:49:04.706850538 +0000
@@ -707,12 +707,14 @@
 				dialog.exec();
 			}
 
+#ifndef __riscos__
 			break;
 		case ActionsManager::CheckForUpdatesAction:
 			{
 				UpdateCheckerDialog *dialog = new UpdateCheckerDialog(this);
 				dialog->show();
 			}
+#endif
 
 			break;
 		case ActionsManager::DiagnosticReportAction:
