--- glib/gutils.c.orig	2021-03-18 13:47:48.344693400 +0000
+++ glib/gutils.c	2022-01-14 11:46:15.232532114 +0000
@@ -2500,7 +2500,11 @@
    */
 #ifndef G_OS_WIN32
   if (!data_dirs || !data_dirs[0])
+#if defined (__riscos__)
+          data_dirs = "/LibGLib2:,/Choices:";
+#else
     data_dirs = "/usr/local/share/:/usr/share/";
+#endif
 
   data_dir_vector = g_strsplit (data_dirs, G_SEARCHPATH_SEPARATOR_S, 0);
 #else
