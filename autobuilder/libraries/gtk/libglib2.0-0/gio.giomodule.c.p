--- gio/giomodule.c.orig	2020-08-02 17:55:12.089956660 +0100
+++ gio/giomodule.c	2020-08-02 17:55:12.121956360 +0100
@@ -1098,6 +1098,12 @@
                                      NULL);
 #endif
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
