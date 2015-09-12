--- CMakeLists.txt.orig	2015-07-17 21:12:12.939351037 +0100
+++ CMakeLists.txt	2015-07-17 21:15:20.687346870 +0100
@@ -70,7 +70,7 @@
 if (EnableQtwebengine)
 	find_package(Qt5 5.5.0 REQUIRED COMPONENTS Core DBus Gui Multimedia Network PrintSupport Script Sql WebEngine WebEngineWidgets WebKit WebKitWidgets Widgets XmlPatterns)
 else (EnableQtwebengine)
-	find_package(Qt5 5.2.0 REQUIRED COMPONENTS Core DBus Gui Multimedia Network PrintSupport Script Sql WebKit WebKitWidgets Widgets XmlPatterns)
+	find_package(Qt5 5.2.0 REQUIRED COMPONENTS Core Gui Network PrintSupport Script Sql WebKit WebKitWidgets Widgets XmlPatterns)
 endif (EnableQtwebengine)
 
 set(otter_src
@@ -320,15 +320,6 @@
 	)
 
 	set_source_files_properties(resources/icons/otter-browser.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
-elseif (UNIX)
-	set(otter_src
-		${otter_src}
-		src/modules/platforms/freedesktoporg/FreeDesktopOrgPlatformIntegration.cpp
-		3rdparty/libmimeapps/ConfigReader.cpp
-		3rdparty/libmimeapps/DesktopEntry.cpp
-		3rdparty/libmimeapps/Index.cpp
-		3rdparty/libmimeapps/Tools.cpp
-	)
 endif (WIN32)
 
 add_executable(otter-browser WIN32 MACOSX_BUNDLE
@@ -357,11 +348,9 @@
 	find_library(FRAMEWORK_Foundation Foundation)
 
 	target_link_libraries(otter-browser ${FRAMEWORK_Cocoa} ${FRAMEWORK_Foundation})
-elseif (UNIX)
-	qt5_use_modules(otter-browser DBus)
 endif (WIN32)
 
-qt5_use_modules(otter-browser Core Gui Multimedia Network PrintSupport Script Sql WebKit WebKitWidgets Widgets XmlPatterns)
+qt5_use_modules(otter-browser Core Gui Network PrintSupport Script Sql WebKit WebKitWidgets Widgets XmlPatterns)
 
 set(OTTER_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
 set(XDG_APPS_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/applications CACHE FILEPATH "Install path for .desktop files")
