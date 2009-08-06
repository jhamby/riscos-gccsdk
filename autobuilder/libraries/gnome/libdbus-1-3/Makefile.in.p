--- Makefile.in.orig	2009-07-31 07:01:44.000000000 -0700
+++ Makefile.in	2009-07-31 07:01:50.000000000 -0700
@@ -347,7 +347,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = dbus bus doc tools test
+SUBDIRS = dbus #bus doc tools test
 DIST_SUBDIRS = dbus bus doc tools test
 pkgconfigdir = $(libdir)/pkgconfig
 pkgconfig_DATA = dbus-1.pc 
--- dbus/Makefile.in.orig	2009-08-05 19:07:50.000000000 -0700
+++ dbus/Makefile.in	2009-08-05 19:08:14.000000000 -0700
@@ -331,7 +331,7 @@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
 configdir = $(sysconfdir)/dbus-1
-INCLUDES = -I$(top_builddir) -I$(top_srcdir) $(DBUS_CLIENT_CFLAGS) @PIC_CFLAGS@ -DDBUS_COMPILATION	\
+INCLUDES = -I$(top_builddir) -I$(top_srcdir) $(DBUS_CLIENT_CFLAGS) -DDBUS_COMPILATION	\
 	-DDBUS_MACHINE_UUID_FILE=\""$(localstatedir)/lib/dbus/machine-id"\"		\
 	-DDBUS_SYSTEM_CONFIG_FILE=\""$(configdir)/system.conf"\"			\
 	-DDBUS_SESSION_CONFIG_FILE=\""$(configdir)/session.conf"\"
