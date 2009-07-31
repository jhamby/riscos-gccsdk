--- dbus/dbus-sysdeps-unix.c.orig	2009-07-31 06:57:37.000000000 -0700
+++ dbus/dbus-sysdeps-unix.c	2009-07-31 06:58:39.000000000 -0700
@@ -1495,6 +1495,7 @@
     struct passwd p_str;
 
     /* retrieve maximum needed size for buf */
+#define _SC_GETPW_R_SIZE_MAX 256
     buflen = sysconf (_SC_GETPW_R_SIZE_MAX);
 
     /* sysconf actually returns a long, but everything else expects size_t,
--- dbus/dbus-sysdeps-util-unix.c.orig	2009-07-31 06:59:29.000000000 -0700
+++ dbus/dbus-sysdeps-util-unix.c	2009-07-31 06:59:58.000000000 -0700
@@ -872,6 +872,7 @@
     dbus_bool_t b;
 
     /* retrieve maximum needed size for buf */
+#define _SC_GETGR_R_SIZE_MAX 256
     buflen = sysconf (_SC_GETGR_R_SIZE_MAX);
 
     /* sysconf actually returns a long, but everything else expects size_t,
