--- glib/gutils.c.orig	2014-05-12 19:08:01.146040349 +0100
+++ glib/gutils.c	2014-05-12 19:08:01.222040350 +0100
@@ -1948,7 +1948,11 @@
       gchar *data_dirs = (gchar *) g_getenv ("XDG_DATA_DIRS");
 
       if (!data_dirs || !data_dirs[0])
+#if defined (__riscos__)
+          data_dirs = "/LibGLib2:,/Choices:";
+#else
           data_dirs = "/usr/local/share/:/usr/share/";
+#endif
 
       data_dir_vector = g_strsplit (data_dirs, G_SEARCHPATH_SEPARATOR_S, 0);
 #endif
