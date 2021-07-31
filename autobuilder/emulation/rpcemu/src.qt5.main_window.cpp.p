--- src/qt5/main_window.cpp.orig	2020-05-09 12:17:07.022383194 +0100
+++ src/qt5/main_window.cpp	2020-05-09 12:40:21.746616197 +0100
@@ -370,11 +370,11 @@
 
 	// MIPS counting
 	window_title.reserve(128);
-	connect(&mips_timer, &QTimer::timeout, this, &MainWindow::mips_timer_timeout);
+	connect(&mips_timer, SIGNAL(timeout()), this, SLOT(mips_timer_timeout()));
 	mips_timer.start(1000);
 	
 	// App losing/gaining focus
-	connect(qApp, &QGuiApplication::applicationStateChanged, this, &MainWindow::application_state_changed);
+	connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(application_state_changed(Qt::ApplicationState)));
 	
 	// Workaround for for qt bug https://bugreports.qt.io/browse/QTBUG-67239
 	// Prevents the menu code stealing the keyboard focus and preventing keys
@@ -472,7 +472,7 @@
 
 	// Disconnect the applicationStateChanged event, because our handler
 	// can generate messages the machine won't be able to handle when quit
-	disconnect(qApp, &QGuiApplication::applicationStateChanged, this, &MainWindow::application_state_changed);
+	disconnect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(application_state_changed(Qt::ApplicationState)));
 
 	// Inform the emulator thread that we're quitting
 	emit this->emulator.exit_signal();
@@ -1007,36 +1007,36 @@
 {
 	// Actions on File menu
 	screenshot_action = new QAction(tr("Take Screenshot..."), this);
-	connect(screenshot_action, &QAction::triggered, this, &MainWindow::menu_screenshot);
+	connect(screenshot_action, SIGNAL(triggered()), this, SLOT(menu_screenshot()));
 	reset_action = new QAction(tr("Reset"), this);
-	connect(reset_action, &QAction::triggered, this, &MainWindow::menu_reset);
+	connect(reset_action, SIGNAL(triggered()), this, SLOT(menu_reset()));
 	exit_action = new QAction(tr("Exit"), this);
 	exit_action->setStatusTip(tr("Exit the application"));
-	connect(exit_action, &QAction::triggered, this, &QMainWindow::close);
+	connect(exit_action, SIGNAL(triggered()), this, SLOT(close()));
 
 	// Actions on Disc->Floppy menu
 	loaddisc0_action = new QAction(tr("Load Drive :0..."), this);
-	connect(loaddisc0_action, &QAction::triggered, this, &MainWindow::menu_loaddisc0);
+	connect(loaddisc0_action, SIGNAL(triggered()), this, SLOT(menu_loaddisc0()));
 	loaddisc1_action = new QAction(tr("Load Drive :1..."), this);
-	connect(loaddisc1_action, &QAction::triggered, this, &MainWindow::menu_loaddisc1);
+	connect(loaddisc1_action, SIGNAL(triggered()), this, SLOT(menu_loaddisc1()));
 
 	// Actions on the Disc->CD-ROM menu
 	cdrom_disabled_action = new QAction(tr("Disabled"), this);
 	cdrom_disabled_action->setCheckable(true);
-	connect(cdrom_disabled_action, &QAction::triggered, this, &MainWindow::menu_cdrom_disabled);
+	connect(cdrom_disabled_action, SIGNAL(triggered()), this, SLOT(menu_cdrom_disabled()));
 
 	cdrom_empty_action = new QAction(tr("Empty"), this);
 	cdrom_empty_action->setCheckable(true);
-	connect(cdrom_empty_action, &QAction::triggered, this, &MainWindow::menu_cdrom_empty);
+	connect(cdrom_empty_action, SIGNAL(triggered()), this, SLOT(menu_cdrom_empty()));
 
 	cdrom_iso_action = new QAction(tr("Iso Image..."), this);
 	cdrom_iso_action->setCheckable(true);
-	connect(cdrom_iso_action, &QAction::triggered, this, &MainWindow::menu_cdrom_iso);
+	connect(cdrom_iso_action, SIGNAL(triggered()), this, SLOT(menu_cdrom_iso()));
 
 #if defined(Q_OS_LINUX)
 	cdrom_ioctl_action = new QAction(tr("Host CD/DVD Drive"), this);
 	cdrom_ioctl_action->setCheckable(true);
-	connect(cdrom_ioctl_action, &QAction::triggered, this, &MainWindow::menu_cdrom_ioctl);
+	connect(cdrom_ioctl_action, SIGNAL(triggered()), this, SLOT(menu_cdrom_ioctl()));
 #endif /* linux */
 #if defined(Q_OS_WIN32)
 	// Dynamically add windows cdrom drives to the settings->cdrom menu
@@ -1058,44 +1058,44 @@
 
 	// Actions on Settings menu
 	configure_action = new QAction(tr("Configure..."), this);
-	connect(configure_action, &QAction::triggered, this, &MainWindow::menu_configure);
+	connect(configure_action, SIGNAL(triggered()), this, SLOT(menu_configure()));
 #ifdef RPCEMU_NETWORKING
 	networking_action = new QAction(tr("Networking..."), this);
-	connect(networking_action, &QAction::triggered, this, &MainWindow::menu_networking);
+	connect(networking_action, SIGNAL(triggered()), this, SLOT(menu_networking()));
 #endif /* RPCEMU_NETWORKING */
 	fullscreen_action = new QAction(tr("Full-screen Mode"), this);
 	fullscreen_action->setCheckable(true);
-	connect(fullscreen_action, &QAction::triggered, this, &MainWindow::menu_fullscreen);
+	connect(fullscreen_action, SIGNAL(triggered()), this, SLOT(menu_fullscreen()));
 	cpu_idle_action = new QAction(tr("Reduce CPU Usage"), this);
 	cpu_idle_action->setCheckable(true);
-	connect(cpu_idle_action, &QAction::triggered, this, &MainWindow::menu_cpu_idle);
+	connect(cpu_idle_action, SIGNAL(triggered()), this, SLOT(menu_cpu_idle()));
 
 	// Actions on the Settings->Mouse menu
 	mouse_hack_action = new QAction(tr("Follow Host Mouse"), this);
 	mouse_hack_action->setCheckable(true);
-	connect(mouse_hack_action, &QAction::triggered, this, &MainWindow::menu_mouse_hack);
+	connect(mouse_hack_action, SIGNAL(triggered()), this, SLOT(menu_mouse_hack()));
 
 	mouse_twobutton_action = new QAction(tr("Two-button Mouse Mode"), this);
 	mouse_twobutton_action->setCheckable(true);
-	connect(mouse_twobutton_action, &QAction::triggered, this, &MainWindow::menu_mouse_twobutton);
+	connect(mouse_twobutton_action, SIGNAL(triggered()), this, SLOT(menu_mouse_twobutton()));
 
 	// Actions on About menu
 	online_manual_action = new QAction(tr("Online Manual..."), this);
-	connect(online_manual_action, &QAction::triggered, this, &MainWindow::menu_online_manual);
+	connect(online_manual_action, SIGNAL(triggered()), this, SLOT(menu_online_manual()));
 	visit_website_action = new QAction(tr("Visit Website..."), this);
-	connect(visit_website_action, &QAction::triggered, this, &MainWindow::menu_visit_website);
+	connect(visit_website_action, SIGNAL(triggered()), this, SLOT(menu_visit_website()));
 
 	about_action = new QAction(tr("About RPCEmu..."), this);
 	about_action->setStatusTip(tr("Show the application's About box"));
-	connect(about_action, &QAction::triggered, this, &MainWindow::menu_about);
+	connect(about_action, SIGNAL(triggered()), this, SLOT(menu_about()));
 
-	connect(this, &MainWindow::main_display_signal, this, &MainWindow::main_display_update, Qt::BlockingQueuedConnection);
+	connect(this, SIGNAL(main_display_signal(VideoUpdate)), this, SLOT(main_display_update(VideoUpdate)), Qt::BlockingQueuedConnection);
 //	connect(this, &MainWindow::main_display_signal, this, &MainWindow::main_display_update);
-	connect(this, &MainWindow::move_host_mouse_signal, this, &MainWindow::move_host_mouse);
+	connect(this, SIGNAL(move_host_mouse_signal(MouseMoveUpdate)), this, SLOT(move_host_mouse(MouseMoveUpdate)));
 
 	// Connections for displaying error messages in the GUI
-	connect(this, &MainWindow::error_signal, this, &MainWindow::error);
-	connect(this, &MainWindow::fatal_signal, this, &MainWindow::fatal);
+	connect(this, SIGNAL(error_signal(QString)), this, SLOT(error(QString)));
+	connect(this, SIGNAL(fatal_signal(QString)), this, SLOT(fatal(QString)));
 }
 
 void
@@ -1187,8 +1187,8 @@
 	for (i = menus.constBegin(); i != menus.constEnd(); i++) {
 		const QMenu *menu = *i;
 
-		connect(menu, &QMenu::aboutToShow, this, &MainWindow::menu_aboutToShow);
-		connect(menu, &QMenu::aboutToHide, this, &MainWindow::menu_aboutToHide);
+		connect(menu, SIGNAL(aboutToShow()), this, SLOT(menu_aboutToShow()));
+		connect(menu, SIGNAL(aboutToHide()), this, SLOT(menu_aboutToHide()));
 	}
 }
 
