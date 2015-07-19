--- src/gui/kernel/qguiapplication.cpp.orig	2015-06-29 21:04:52.000000000 +0100
+++ src/gui/kernel/qguiapplication.cpp	2015-07-18 18:56:25.192146350 +0100
@@ -1173,6 +1173,12 @@
         argv[j] = 0;
         argc = j;
     }
+ 
+#ifdef Q_OS_RISCOS
+    if (platformPluginPath.isEmpty()) {
+	platformPluginPath = QLatin1String("/SharedLibs:lib/abi-2.0/Qt/plugins/platforms");
+    }
+#endif
 
     init_platform(QLatin1String(platformName), platformPluginPath, platformThemeName, argc, argv);
 
