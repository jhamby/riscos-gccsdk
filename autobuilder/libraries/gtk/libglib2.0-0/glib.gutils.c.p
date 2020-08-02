--- glib/gutils.c.orig	2020-08-02 17:55:12.081956735 +0100
+++ glib/gutils.c	2020-08-02 17:55:12.113956435 +0100
@@ -2051,7 +2051,11 @@
 
 #ifndef G_OS_WIN32
       if (!data_dirs || !data_dirs[0])
+#if defined (__riscos__)
+          data_dirs = "/LibGLib2:,/Choices:";
+#else
           data_dirs = "/usr/local/share/:/usr/share/";
+#endif
 
       data_dir_vector = g_strsplit (data_dirs, G_SEARCHPATH_SEPARATOR_S, 0);
 #else
