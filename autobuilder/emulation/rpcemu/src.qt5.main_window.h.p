--- src/qt5/main_window.h.orig	2020-05-09 12:39:41.087424766 +0100
+++ src/qt5/main_window.h	2020-05-09 12:40:21.746616197 +0100
@@ -99,6 +99,7 @@
 	MainWindow(Emulator &emulator);
 	virtual ~MainWindow();
 
+public slots:
 	/* Handle displaying error messages */
 	void error(QString error);
 	void fatal(QString error);
