--- gtk/xdgmime/xdgmime.c.orig	2008-04-26 23:23:24.000000000 -0700
+++ gtk/xdgmime/xdgmime.c	2008-04-26 23:13:03.000000000 -0700
@@ -57,6 +57,9 @@
 static XdgDirTimeList *dir_time_list = NULL;
 static XdgCallbackList *callback_list = NULL;
 
+
+extern XdgMimeCache **_caches __attribute__ ((weak));
+
 XdgMimeCache **_caches = NULL;
 static int n_caches = 0;
 
