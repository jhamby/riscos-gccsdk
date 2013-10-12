--- glib/gutils.c.orig	2013-10-09 20:24:14.461124636 +0100
+++ glib/gutils.c	2013-10-09 20:24:14.505124636 +0100
@@ -2017,7 +2017,11 @@
       gchar *data_dirs = (gchar *) g_getenv ("XDG_DATA_DIRS");
 
       if (!data_dirs || !data_dirs[0])
+#if defined (__riscos__)
+          data_dirs = "/LibGLib2:,/Choices:";
+#else
           data_dirs = "/usr/local/share/:/usr/share/";
+#endif
 
       data_dir_vector = g_strsplit (data_dirs, G_SEARCHPATH_SEPARATOR_S, 0);
 #endif
