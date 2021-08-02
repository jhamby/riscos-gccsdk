--- src/gui/kernel/qguiapplication.cpp.orig	2015-06-29 21:04:52.000000000 +0100
+++ src/gui/kernel/qguiapplication.cpp	2015-07-25 00:56:11.088064012 +0100
@@ -1173,6 +1173,16 @@
         argv[j] = 0;
         argc = j;
     }
+ 
+#ifdef Q_OS_RISCOS
+    if (platformPluginPath.isEmpty()) {
+#if (defined(__VFP_FP__) && !defined(__SOFTFP__)) && 0
+	platformPluginPath = QLatin1String("/SharedLibs:lib/armeabihf/vfp/Qt/plugins/platforms");
+#else
+	platformPluginPath = QLatin1String("/SharedLibs:lib/armeabihf/Qt/plugins/platforms");
+#endif
+    }
+#endif
 
     init_platform(QLatin1String(platformName), platformPluginPath, platformThemeName, argc, argv);
 
