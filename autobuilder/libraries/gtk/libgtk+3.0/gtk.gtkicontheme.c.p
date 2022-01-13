--- gtk/gtkicontheme.c.orig	2020-06-11 03:45:39.000000000 +0100
+++ gtk/gtkicontheme.c	2020-11-25 20:11:54.525006032 +0000
@@ -732,10 +732,16 @@
   for (i = 0; xdg_data_dirs[i]; i++) ;
 
   priv->search_path_len = 2 * i + 2;
+#ifdef __riscos__
+  priv->search_path_len++;
+#endif
   
   priv->search_path = g_new (char *, priv->search_path_len);
   
   i = 0;
+#ifdef __riscos__
+  priv->search_path[i++] = g_strdup("/Choices:gtk/icons");
+#endif
   priv->search_path[i++] = g_build_filename (g_get_user_data_dir (), "icons", NULL);
   priv->search_path[i++] = g_build_filename (g_get_home_dir (), ".icons", NULL);
   
