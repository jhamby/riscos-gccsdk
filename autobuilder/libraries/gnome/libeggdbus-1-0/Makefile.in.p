--- src/eggdbus/Makefile.in.orig	2009-11-24 12:38:08.000000000 -0800
+++ src/eggdbus/Makefile.in	2009-11-24 12:39:17.000000000 -0800
@@ -1370,12 +1370,13 @@
 	uninstall-libLTLIBRARIES uninstall-libeggdbus_1includeHEADERS
 
 
-eggdbus-built-sources.stamp : eggdbus-binding-tool Makefile.am org.freedesktop.DBus.xml org.freedesktop.DBus.Peer.xml org.freedesktop.DBus.Properties.xml org.freedesktop.DBus.Introspectable.xml eggdbus-glib-genmarshal
+eggdbus-built-sources.stamp : eggdbus-binding-tool$(EXEEXT) Makefile.am org.freedesktop.DBus.xml org.freedesktop.DBus.Peer.xml org.freedesktop.DBus.Properties.xml org.freedesktop.DBus.Introspectable.xml eggdbus-glib-genmarshal$(EXEEXT)
 	EGG_DBUS_GENMARSHAL=$(top_builddir)/src/eggdbus/eggdbus-glib-genmarshal $(top_builddir)/src/eggdbus/eggdbus-binding-tool							\
 		--namespace EggDBus									\
 		--dbus-namespace org.freedesktop.DBus							\
 		--interfaces-only									\
 		--no-types-header									\
+		--include-header "sys/types.h"								\
 		--include-header "eggdbus/eggdbustypes.h"						\
 		--include-header "eggdbus/eggdbusinterface.h"						\
 		--include-header "eggdbus/eggdbusinterfaceproxy.h"					\
