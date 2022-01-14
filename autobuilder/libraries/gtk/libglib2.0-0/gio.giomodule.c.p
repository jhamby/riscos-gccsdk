--- gio/giomodule.c.orig	2021-03-18 13:47:48.272693200 +0000
+++ gio/giomodule.c	2022-01-14 11:46:15.228532142 +0000
@@ -1177,6 +1177,12 @@
                                      "lib", "gio", "modules",
                                      NULL);
       g_free (install_dir);
+#elif defined(__riscos__)
+#ifdef __ARM_EABI__
+      module_dir = g_strdup ("/SharedLibs:lib/armeabihf/gio/modules");
+#else
+      module_dir = g_strdup ("/SharedLibs:lib/abi-2.0/gio/modules");
+#endif
 #else
       module_dir = g_strdup (GIO_MODULE_DIR);
 #endif
