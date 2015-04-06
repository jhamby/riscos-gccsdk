--- src/gui/kernel/qguiapplication.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/gui/kernel/qguiapplication.cpp	2014-05-27 20:38:56.546556795 +0100
@@ -1124,6 +1124,12 @@
         argv[j] = 0;
         argc = j;
     }
+ 
+#ifdef __riscos__
+    if (platformPluginPath.isEmpty()) {
+	platformPluginPath = QLatin1String("/SharedLibs:lib/abi-2.0/Qt/plugins/platforms");
+    }
+#endif
 
     init_platform(QLatin1String(platformName), platformPluginPath, platformThemeName, argc, argv);
 
