--- src/qt5/rpc-qt5.cpp.orig	2020-05-09 12:21:32.617095184 +0100
+++ src/qt5/rpc-qt5.cpp	2020-05-09 12:40:21.750616119 +0100
@@ -434,10 +434,10 @@
 
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
@@ -557,9 +557,11 @@
 		// If NAT networking, poll, but not too often
 		if (config.network_type == NetworkType_NAT) {
 			network_nat_rate++;
+#ifndef __riscos__
 			if ((network_nat_rate & 0x3) == 0) {
 				network_nat_poll();
 			}
+#endif
 		}
 #endif
 	}
@@ -925,10 +925,11 @@
 
 	QByteArray ba_ipaddress = ipaddress.toUtf8();
 	const char *ip_address = ba_ipaddress.constData();
-
+#ifndef __riscos__
 	if (network_config_changed(network_type, bridge_name, ip_address)) {
 		this->reset();
 	}
+#endif
 }
 
 /**
