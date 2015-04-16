--- src/gui/kernel/qguiapplication.cpp.orig	2015-02-17 04:56:48.000000000 +0000
+++ src/gui/kernel/qguiapplication.cpp	2015-04-12 13:43:51.548657029 +0100
@@ -1161,6 +1161,12 @@
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
 
