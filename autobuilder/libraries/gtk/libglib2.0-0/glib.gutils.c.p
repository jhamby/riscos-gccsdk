--- glib/gutils.c.orig	2017-04-01 20:02:42.372064629 +0100
+++ glib/gutils.c	2017-04-01 20:02:42.472064630 +0100
@@ -1967,7 +1967,11 @@
       gchar *data_dirs = (gchar *) g_getenv ("XDG_DATA_DIRS");
 
       if (!data_dirs || !data_dirs[0])
+#if defined (__riscos__)
+          data_dirs = "/LibGLib2:,/Choices:";
+#else
           data_dirs = "/usr/local/share/:/usr/share/";
+#endif
 
       data_dir_vector = g_strsplit (data_dirs, G_SEARCHPATH_SEPARATOR_S, 0);
 #endif
