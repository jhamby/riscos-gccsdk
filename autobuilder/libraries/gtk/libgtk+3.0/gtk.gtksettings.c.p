diff --git a/gtk/gtksettings.c b/gtk/gtksettings.c
index 0f60dcdfe4..e4e512f887 100644
--- gtk/gtksettings.c
+++ gtk/gtksettings.c
@@ -342,7 +342,11 @@ gtk_settings_init (GtkSettings *settings)
     }
   g_free (pspecs);
 
+#ifdef __riscos__
+  path = g_build_filename ("/<Choices$Dir>/gtk", "gtk-3.0", "settings.ini", NULL);
+#else
   path = g_build_filename (_gtk_get_data_prefix (), "share", "gtk-3.0", "settings.ini", NULL);
+#endif
   if (g_file_test (path, G_FILE_TEST_EXISTS))
     gtk_settings_load_from_key_file (settings, path, GTK_SETTINGS_SOURCE_DEFAULT);
   g_free (path);
@@ -1881,10 +1885,17 @@ settings_init_style (GtkSettings *settings)
       gchar *css_path;
 
       css_provider = gtk_css_provider_new ();
+#ifdef __riscos__
+      css_path = g_build_filename ("/<Choices$Dir>/gtk",
+                                   "gtk-3.0",
+                                   "gtk.css",
+                                   NULL);
+#else
       css_path = g_build_filename (g_get_user_config_dir (),
                                    "gtk-3.0",
                                    "gtk.css",
                                    NULL);
+#endif
 
       if (g_file_test (css_path,
                        G_FILE_TEST_IS_REGULAR))
