--- src/eggdbus/Makefile.in.orig	2009-11-24 10:09:33.000000000 -0800
+++ src/eggdbus/Makefile.in	2009-11-24 10:10:43.000000000 -0800
@@ -1298,7 +1298,7 @@
 	uninstall-libLTLIBRARIES uninstall-libeggdbus_1includeHEADERS
 
 
-eggdbus-built-sources.stamp : eggdbus-binding-tool Makefile.am org.freedesktop.DBus.xml org.freedesktop.DBus.Peer.xml org.freedesktop.DBus.Properties.xml org.freedesktop.DBus.Introspectable.xml eggdbus-glib-genmarshal
+eggdbus-built-sources.stamp : eggdbus-binding-tool$(EXEEXT) Makefile.am org.freedesktop.DBus.xml org.freedesktop.DBus.Peer.xml org.freedesktop.DBus.Properties.xml org.freedesktop.DBus.Introspectable.xml eggdbus-glib-genmarshal
 	EGG_DBUS_GENMARSHAL=$(top_builddir)/src/eggdbus/eggdbus-glib-genmarshal $(top_builddir)/src/eggdbus/eggdbus-binding-tool							\
 		--namespace EggDBus									\
 		--dbus-namespace org.freedesktop.DBus							\
