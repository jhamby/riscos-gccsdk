diff -r f563d0ae0170 src/qt5/about_dialog.cpp
--- a/src/qt5/about_dialog.cpp	Sun Dec 31 20:22:10 2017 +0000
+++ b/src/qt5/about_dialog.cpp	Mon Jan 01 13:14:17 2018 +0000
@@ -68,7 +68,7 @@
 	// Remove resize on Dialog
 	this->setFixedSize(this->sizeHint());
 
-	connect(buttons_box, &QDialogButtonBox::accepted, this, &QDialog::close);
+	connect(buttons_box, SIGNAL(accepted()), this, SLOT(close()));
 }
 
 AboutDialog::~AboutDialog()
diff -r f563d0ae0170 src/qt5/configure_dialog.cpp
--- a/src/qt5/configure_dialog.cpp	Sun Dec 31 20:22:10 2017 +0000
+++ b/src/qt5/configure_dialog.cpp	Mon Jan 01 13:14:17 2018 +0000
@@ -129,13 +129,13 @@
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
diff -r f563d0ae0170 src/qt5/main_window.cpp
--- a/src/qt5/main_window.cpp	Sun Dec 31 20:22:10 2017 +0000
+++ b/src/qt5/main_window.cpp	Mon Jan 01 13:14:17 2018 +0000
@@ -27,6 +27,7 @@
 #include <QMenuBar>
 #include <QMessageBox>
 #include <QPainter>
+#include <QShortcut>
 
 #if defined(Q_OS_WIN32)
 #include "Windows.h"
@@ -346,11 +347,11 @@
 
 	// MIPS counting
 	window_title.reserve(128);
-	connect(&mips_timer, &QTimer::timeout, this, &MainWindow::mips_timer_timeout);
+	connect(&mips_timer, SIGNAL(timeout()), this, SLOT(mips_timer_timeout()));
 	mips_timer.start(1000);
 	
 	// App losing/gaining focus
-	connect(qApp, &QGuiApplication::applicationStateChanged, this, &MainWindow::application_state_changed);
+	connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(application_state_changed(Qt::ApplicationState)));
 }
 
 MainWindow::~MainWindow()
@@ -811,48 +812,49 @@
 MainWindow::create_actions()
 {
 	// Actions on File menu
-	reset_action = new QAction(tr("Reset"), this);
-	connect(reset_action, &QAction::triggered, this, &MainWindow::menu_reset);
+	reset_action = new QAction(tr("&Reset"), this);
+	connect(reset_action, SIGNAL(triggered()), this, SLOT(menu_reset()));
 
-	exit_action = new QAction(tr("Exit"), this);
+	exit_action = new QAction(tr("E&xit"), this);
+	exit_action->setShortcuts(QKeySequence::Quit);
 	exit_action->setStatusTip(tr("Exit the application"));
-	connect(exit_action, &QAction::triggered, this, &QMainWindow::close);
+	connect(exit_action, SIGNAL(triggered()), this, SLOT(close()));
 
 	// Actions on Disc menu
 	loaddisc0_action = new QAction(tr("Load Disc :0"), this);
-	connect(loaddisc0_action, &QAction::triggered, this, &MainWindow::menu_loaddisc0);
+	connect(loaddisc0_action, SIGNAL(triggered()), this, SLOT(menu_loaddisc0()));
 	loaddisc1_action = new QAction(tr("Load Disc :1"), this);
-	connect(loaddisc1_action, &QAction::triggered, this, &MainWindow::menu_loaddisc1);
+	connect(loaddisc1_action, SIGNAL(triggered()), this, SLOT(menu_loaddisc1()));
 
 	// Actions on Settings menu
-	configure_action = new QAction(tr("Configure..."), this);
-	connect(configure_action, &QAction::triggered, this, &MainWindow::menu_configure);
+	configure_action = new QAction(tr("&Configure..."), this);
+	connect(configure_action, SIGNAL(triggered()), this, SLOT(menu_configure()));
 #ifdef RPCEMU_NETWORKING
-	networking_action = new QAction(tr("Networking..."), this);
+	networking_action = new QAction(tr("&Networking..."), this);
 	connect(networking_action, &QAction::triggered, this, &MainWindow::menu_networking);
 #endif /* RPCEMU_NETWORKING */
-	fullscreen_action = new QAction(tr("Fullscreen mode"), this);
+	fullscreen_action = new QAction(tr("&Fullscreen mode"), this);
 	fullscreen_action->setCheckable(true);
-	connect(fullscreen_action, &QAction::triggered, this, &MainWindow::menu_fullscreen);
-	cpu_idle_action = new QAction(tr("Reduce CPU usage"), this);
+	connect(fullscreen_action, SIGNAL(triggered()), this, SLOT(menu_fullscreen()));
+	cpu_idle_action = new QAction(tr("&Reduce CPU usage"), this);
 	cpu_idle_action->setCheckable(true);
-	connect(cpu_idle_action, &QAction::triggered, this, &MainWindow::menu_cpu_idle);
+	connect(cpu_idle_action, SIGNAL(triggered()), this, SLOT(menu_cpu_idle()));
 
 	// Actions on the Settings->CD ROM Menu
-	cdrom_disabled_action = new QAction(tr("Disabled"), this);
+	cdrom_disabled_action = new QAction(tr("&Disabled"), this);
 	cdrom_disabled_action->setCheckable(true);
-	connect(cdrom_disabled_action, &QAction::triggered, this, &MainWindow::menu_cdrom_disabled);
+	connect(cdrom_disabled_action, SIGNAL(triggered()), this, SLOT(menu_cdrom_disabled()));
 
-	cdrom_empty_action = new QAction(tr("Empty"), this);
+	cdrom_empty_action = new QAction(tr("&Empty"), this);
 	cdrom_empty_action->setCheckable(true);
-	connect(cdrom_empty_action, &QAction::triggered, this, &MainWindow::menu_cdrom_empty);
+	connect(cdrom_empty_action, SIGNAL(triggered()), this, SLOT(menu_cdrom_empty()));
 
-	cdrom_iso_action = new QAction(tr("Iso Image..."), this);
+	cdrom_iso_action = new QAction(tr("&Iso Image..."), this);
 	cdrom_iso_action->setCheckable(true);
-	connect(cdrom_iso_action, &QAction::triggered, this, &MainWindow::menu_cdrom_iso);
+	connect(cdrom_iso_action, SIGNAL(triggered()), this, SLOT(menu_cdrom_iso()));
 
 #if defined(Q_OS_LINUX)
-	cdrom_ioctl_action = new QAction(tr("Host CD/DVD Drive"), this);
+	cdrom_ioctl_action = new QAction(tr("&Host CD/DVD Drive"), this);
 	cdrom_ioctl_action->setCheckable(true);
 	connect(cdrom_ioctl_action, &QAction::triggered, this, &MainWindow::menu_cdrom_ioctl);
 #endif /* linux */
@@ -878,48 +880,49 @@
 #endif
 
 	// Actions on the Settings->Mouse menu
-	mouse_hack_action = new QAction(tr("Follow host mouse"), this);
+	mouse_hack_action = new QAction(tr("&Follow host mouse"), this);
 	mouse_hack_action->setCheckable(true);
-	connect(mouse_hack_action, &QAction::triggered, this, &MainWindow::menu_mouse_hack);
+	connect(mouse_hack_action, SIGNAL(triggered()), this, SLOT(menu_mouse_hack()));
 
-	mouse_twobutton_action = new QAction(tr("Two-button Mouse Mode"), this);
+	mouse_twobutton_action = new QAction(tr("&Two-button Mouse Mode"), this);
 	mouse_twobutton_action->setCheckable(true);
-	connect(mouse_twobutton_action, &QAction::triggered, this, &MainWindow::menu_mouse_twobutton);
+	connect(mouse_twobutton_action, SIGNAL(triggered()), this, SLOT(menu_mouse_twobutton()));
 
 	// Actions on About menu
-	online_manual_action = new QAction(tr("Online Manual..."), this);
-	connect(online_manual_action, &QAction::triggered, this, &MainWindow::menu_online_manual);
-	visit_website_action = new QAction(tr("Visit Website..."), this);
-	connect(visit_website_action, &QAction::triggered, this, &MainWindow::menu_visit_website);
+	online_manual_action = new QAction(tr("Online &Manual..."), this);
+	connect(online_manual_action, SIGNAL(triggered()), this, SLOT(menu_online_manual()));
+	visit_website_action = new QAction(tr("Visit &Website..."), this);
+	connect(visit_website_action, SIGNAL(triggered()), this, SLOT(menu_visit_website()));
 
-	about_action = new QAction(tr("About RPCEmu..."), this);
+	about_action = new QAction(tr("&About RPCEmu..."), this);
 	about_action->setStatusTip(tr("Show the application's About box"));
-	connect(about_action, &QAction::triggered, this, &MainWindow::menu_about);
+	connect(about_action, SIGNAL(triggered()), this, SLOT(menu_about()));
 
-	connect(this, &MainWindow::main_display_signal, this, &MainWindow::main_display_update, Qt::BlockingQueuedConnection);
-//	connect(this, &MainWindow::main_display_signal, this, &MainWindow::main_display_update);
+	connect(this, SIGNAL(main_display_signal(VideoUpdate)), this, SLOT(main_display_update(VideoUpdate)),
+		Qt::BlockingQueuedConnection);
+//	connect(this, SIGNAL(main_display_signal(VideoUpdate)), this, SLOT(main_display_update(VideoUpdate)));
 
 	// Connections for displaying error messages in the GUI
-	connect(this, &MainWindow::error_signal, this, &MainWindow::error);
-	connect(this, &MainWindow::fatal_signal, this, &MainWindow::fatal);
+	connect(this, SIGNAL(error_signal(QString)), this, SLOT(error(QString)));
+	connect(this, SIGNAL(fatal_signal(QString)), this, SLOT(fatal(QString)));
 }
 
 void
 MainWindow::create_menus()
 {
 	// File menu
-	file_menu = menuBar()->addMenu(tr("File"));
+	file_menu = menuBar()->addMenu(tr("&File"));
 	file_menu->addAction(reset_action);
 	file_menu->addSeparator();
 	file_menu->addAction(exit_action);
 
 	// Disc menu
-	disc_menu = menuBar()->addMenu(tr("Disc"));
+	disc_menu = menuBar()->addMenu(tr("&Disc"));
 	disc_menu->addAction(loaddisc0_action);
 	disc_menu->addAction(loaddisc1_action);
 
 	// Settings menu (and submenus)
-	settings_menu = menuBar()->addMenu(tr("Settings"));
+	settings_menu = menuBar()->addMenu(tr("&Settings"));
 	settings_menu->addAction(configure_action);
 #ifdef RPCEMU_NETWORKING
 	settings_menu->addAction(networking_action);
@@ -928,9 +931,9 @@
 	settings_menu->addAction(fullscreen_action);
 	settings_menu->addAction(cpu_idle_action);
 	settings_menu->addSeparator();
-	cdrom_menu = settings_menu->addMenu(tr("CD-ROM"));
+	cdrom_menu = settings_menu->addMenu(tr("&CD-ROM"));
 	settings_menu->addSeparator();
-	mouse_menu = settings_menu->addMenu(tr("Mouse"));
+	mouse_menu = settings_menu->addMenu(tr("&Mouse"));
 
 	// CD-ROM submenu
 	cdrom_menu->addAction(cdrom_disabled_action);
@@ -953,7 +956,7 @@
 	menuBar()->addSeparator();
 
 	// Help menu
-	help_menu = menuBar()->addMenu(tr("Help"));
+	help_menu = menuBar()->addMenu(tr("&Help"));
 	help_menu->addAction(online_manual_action);
 	help_menu->addAction(visit_website_action);
 	help_menu->addSeparator();
@@ -1133,46 +1136,3 @@
 #endif
 	}
 }
-
-#if defined(Q_OS_WIN32)
-/**
- * windows pre event handler used by us to modify some default behaviour
- *
- * Disable the use of the virtual menu key (alt) that otherwise goes off
- * every time someone presses alt in the emulated OS
- *
- * @param eventType unused
- * @param message window event MSG struct
- * @param result unused
- * @return bool of whether we've handled the event (true) or windows/qt should deal with it (false) 
- */
-bool
-MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
-{
-	Q_UNUSED(result);
-	Q_UNUSED(eventType);
-
-	MSG *msg = static_cast<MSG*>(message);
-
-	// Handle 'alt' key presses that would select the menu
-	// Fake 'alt' key press and release and then tell windows/qt to
-	// not handle it
-	if((msg->message == WM_SYSKEYDOWN || msg->message == WM_SYSKEYUP)
-	    && (msg->wParam == VK_MENU))
-	{
-		// Use the code from the qt key press and release handlers
-		// 0x38 is the windows native keycode for alt
-		if(msg->message == WM_SYSKEYDOWN) {
-			held_keys.insert(held_keys.end(), 0x38);
-			emit this->emulator.key_press_signal(0x38);
-		} else {
-			held_keys.remove(0x38);
-			emit this->emulator.key_release_signal(0x38);
-		}
-		return true;
-	}
-
-	// Anything else should be handled by the regular qt and windows handlers
-	return false;
-}
-#endif // Q_OS_WIN32
diff -r f563d0ae0170 src/qt5/main_window.h
--- a/src/qt5/main_window.h	Sun Dec 31 20:22:10 2017 +0000
+++ b/src/qt5/main_window.h	Mon Jan 01 13:14:17 2018 +0000
@@ -86,6 +86,7 @@
 	virtual ~MainWindow();
 
 	/* Handle displaying error messages */
+public slots:
 	void error(QString error);
 	void fatal(QString error);
 
diff -r f563d0ae0170 src/qt5/network_dialog.cpp
--- a/src/qt5/network_dialog.cpp	Sun Dec 31 20:22:10 2017 +0000
+++ b/src/qt5/network_dialog.cpp	Mon Jan 01 13:14:17 2018 +0000
@@ -73,15 +73,15 @@
 	vbox->addWidget(buttons_box);
 
 	// Connect actions to widgets
-	connect(net_off, &QRadioButton::clicked, this, &NetworkDialog::radio_clicked);
-	connect(net_bridging, &QRadioButton::clicked, this, &NetworkDialog::radio_clicked);
-	connect(net_tunnelling, &QRadioButton::clicked, this, &NetworkDialog::radio_clicked);
+	connect(net_off, SIGNAL(clicked()), this, SLOT(radio_clicked()));
+	connect(net_bridging, SIGNAL(clicked()), this, SLOT(radio_clicked()));
+	connect(net_tunnelling, SIGNAL(clicked()), this, SLOT(radio_clicked()));
 
-	connect(buttons_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
-	connect(buttons_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
+	connect(buttons_box, SIGNAL(accepted()), this, SLOT(accept()));
+	connect(buttons_box, SIGNAL(rejected()), this, SLOT(reject()));
 
-	connect(this, &QDialog::accepted, this, &NetworkDialog::dialog_accepted);
-	connect(this, &QDialog::rejected, this, &NetworkDialog::dialog_rejected);
+	connect(this, SIGNAL(accepted()), this, SLOT(dialog_accepted()));
+	connect(this, SIGNAL(rejected()), this, SLOT(dialog_rejected()));
 
 	// Set the values of the window to the config values
 	applyConfig();
diff -r f563d0ae0170 src/qt5/rpc-qt5.cpp
--- a/src/qt5/rpc-qt5.cpp	Sun Dec 31 20:22:10 2017 +0000
+++ b/src/qt5/rpc-qt5.cpp	Mon Jan 01 13:14:17 2018 +0000
@@ -408,10 +408,10 @@
 
 	emulator = new Emulator;
 	emulator->moveToThread(emu_thread);
-	QThread::connect(emu_thread, &QThread::started, emulator, &Emulator::mainemuloop);
-	QThread::connect(emulator, &Emulator::finished, emu_thread, &QThread::quit);
-	QThread::connect(emulator, &Emulator::finished, emulator, &Emulator::deleteLater);
-	QThread::connect(emu_thread, &QThread::finished, emu_thread, &QThread::deleteLater);
+	QThread::connect(emu_thread, SIGNAL(started()), emulator, SLOT(mainemuloop()));
+	QThread::connect(emulator, SIGNAL(finished()), emu_thread, SLOT(quit()));
+	QThread::connect(emulator, SIGNAL(finished()), emulator, SLOT(deleteLater()));
+	QThread::connect(emu_thread, SIGNAL(finished()), emu_thread, SLOT(deleteLater()));
 
 	// Create Main Window
 	MainWindow main_window(*emulator);
@@ -442,35 +442,35 @@
 Emulator::Emulator()
 {
 	// Signals from the main GUI window to provide emulated machine input
-	connect(this, &Emulator::key_press_signal,
-	        this, &Emulator::key_press);
+	connect(this, SIGNAL(key_press_signal(unsigned)),
+	        this, SLOT(key_press(unsigned)));
 
-	connect(this, &Emulator::key_release_signal,
-	        this, &Emulator::key_release);
+	connect(this, SIGNAL(key_release_signal(unsigned)),
+	        this, SLOT(key_release(unsigned)));
 
-	connect(this, &Emulator::mouse_move_signal, this, &Emulator::mouse_move);
-	connect(this, &Emulator::mouse_move_relative_signal, this, &Emulator::mouse_move_relative);
-	connect(this, &Emulator::mouse_press_signal, this, &Emulator::mouse_press);
-	connect(this, &Emulator::mouse_release_signal, this, &Emulator::mouse_release);
+	connect(this, SIGNAL(mouse_move_signal(int, int)), this, SLOT(mouse_move(int, int)));
+	connect(this, SIGNAL(mouse_move_relative_signal(int, int)), this, SLOT(mouse_move_relative(int, int)));
+	connect(this, SIGNAL(mouse_press_signal(int)), this, SLOT(mouse_press(int)));
+	connect(this, SIGNAL(mouse_release_signal(int)), this, SLOT(mouse_release(int)));
 
 	// Signals from user GUI interactions to control parts of the emulator
-	connect(this, &Emulator::reset_signal, this, &Emulator::reset);
-	connect(this, &Emulator::exit_signal, this, &Emulator::exit);
-	connect(this, &Emulator::load_disc_0_signal, this, &Emulator::load_disc_0);
-	connect(this, &Emulator::load_disc_1_signal, this, &Emulator::load_disc_1);
-	connect(this, &Emulator::cpu_idle_signal, this, &Emulator::cpu_idle);
-	connect(this, &Emulator::cdrom_disabled_signal, this, &Emulator::cdrom_disabled);
-	connect(this, &Emulator::cdrom_empty_signal, this, &Emulator::cdrom_empty);
-	connect(this, &Emulator::cdrom_load_iso_signal, this, &Emulator::cdrom_load_iso);
+	connect(this, SIGNAL(reset_signal()), this, SLOT(reset()));
+	connect(this, SIGNAL(exit_signal()), this, SLOT(exit()));
+	connect(this, SIGNAL(load_disc_0_signal(QString)), this, SLOT(load_disc_0(QString)));
+	connect(this, SIGNAL(load_disc_1_signal(QString)), this, SLOT(load_disc_1(QString)));
+	connect(this, SIGNAL(cpu_idle_signal()), this, SLOT(cpu_idle()));
+	connect(this, SIGNAL(cdrom_disabled_signal()), this, SLOT(cdrom_disabled()));
+	connect(this, SIGNAL(cdrom_empty_signal()), this, SLOT(cdrom_empty()));
+	connect(this, SIGNAL(cdrom_load_iso_signal(QString)), this, SLOT(cdrom_load_iso(QString)));
 #if defined(Q_OS_LINUX)
 	connect(this, &Emulator::cdrom_ioctl_signal, this, &Emulator::cdrom_ioctl);
 #endif // linux
 #if defined(Q_OS_WIN32)
 	connect(this, &Emulator::cdrom_win_ioctl_signal, this, &Emulator::cdrom_win_ioctl);
 #endif // win32
-	connect(this, &Emulator::mouse_hack_signal, this, &Emulator::mouse_hack);
-	connect(this, &Emulator::mouse_twobutton_signal, this, &Emulator::mouse_twobutton);
-	connect(this, &Emulator::config_updated_signal, this, &Emulator::config_updated);
+	connect(this, SIGNAL(mouse_hack_signal()), this, SLOT(mouse_hack()));
+	connect(this, SIGNAL(mouse_twobutton_signal()), this, SLOT(mouse_twobutton()));
+	connect(this, SIGNAL(config_updated_signal(Config*, Model)), this, SLOT(config_updated(Config*, Model)));
 }
 
 /**
diff -r f563d0ae0170 src/romload.c
--- a/src/romload.c	Sun Dec 31 20:22:10 2017 +0000
+++ b/src/romload.c	Mon Jan 01 13:14:17 2018 +0000
@@ -61,6 +61,7 @@
 	{ 0xe504,  { 0xe3a00402, 0xe2801004, 0xeb0001ad, 0x03a06002 }, 0xe510,  0x03a06008, "8MB VRAM RISC OS 4.37" },
 	{ 0xe248,  { 0xe3a00402, 0xe2801004, 0xeb0001ae, 0x03a06002 }, 0xe254,  0x03a06008, "8MB VRAM RISC OS 4.39" },
 	{ 0x8a764, { 0xe1a00001, 0xe2801004, 0xeb00000d, 0x03a06002 }, 0x8a770, 0x03a06008, "8MB VRAM RISC OS 6.02" },
+	{ 0x1a460, { 0xe328f202, 0xe8bd803f, 0x00000000, 0x20000000 }, 0x1a46c, 0x01c00000, "28MB WimpSlot Aemulor" },
 };
 
 /**
diff -r f563d0ae0170 src/rpc-linux.c
--- a/src/rpc-linux.c	Sun Dec 31 20:22:10 2017 +0000
+++ b/src/rpc-linux.c	Mon Jan 01 13:14:17 2018 +0000
@@ -24,7 +24,9 @@
 #include <string.h>
 
 #include <pthread.h>
+#ifndef __riscos__
 #include <sys/statvfs.h>
+#endif
 #include <sys/types.h>
 #include <sys/utsname.h>
 #include <sys/wait.h>
@@ -47,6 +49,7 @@
 int
 path_disk_info(const char *path, disk_info *d)
 {
+#ifndef __riscos__
 	struct statvfs s;
 	int ret;
 
@@ -61,6 +64,9 @@
 	d->free = (uint64_t) s.f_bavail * (uint64_t) s.f_frsize;
 
 	return 1;
+#else
+	return 0;
+#endif
 }
 
 /**
