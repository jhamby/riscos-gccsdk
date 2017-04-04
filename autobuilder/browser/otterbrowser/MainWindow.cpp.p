--- src/ui/MainWindow.cpp.orig	2017-04-04 19:49:20.610324959 +0100
+++ src/ui/MainWindow.cpp	2017-04-04 19:58:29.948026052 +0100
@@ -984,6 +984,7 @@
 			}
 
 			return;
+#ifndef __riscos__
 		case ActionsManager::CheckForUpdatesAction:
 			{
 				UpdateCheckerDialog *dialog(new UpdateCheckerDialog(this));
@@ -991,6 +992,7 @@
 			}
 
 			return;
+#endif
 		case ActionsManager::DiagnosticReportAction:
 			{
 				ReportDialog *dialog(new ReportDialog(Application::FullReport, this));
