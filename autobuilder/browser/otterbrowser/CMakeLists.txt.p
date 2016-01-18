--- CMakeLists.txt.orig	2016-01-18 20:07:34.522694170 +0000
+++ CMakeLists.txt	2016-01-18 20:13:07.466694510 +0000
@@ -15,7 +15,8 @@
 set(PATCH_VERSION "10")
 set(OTTER_VERSION_MAIN ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
 set(OTTER_VERSION_WEEKLY " ")
-set(OTTER_VERSION_CONTEXT "-dev")
+string(TIMESTAMP DATE_TIMESTAMP %Y%m%d)
+set(OTTER_VERSION_CONTEXT "-dev" ${DATE_TIMESTAMP})
 
 if (EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
 	include(InstallRequiredSystemLibraries)
@@ -32,7 +33,7 @@
 	set(CPACK_PACKAGE_NAME "otter-browser")
 	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
 	set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
-	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5webkit5 (>=5.1.0), libqt5script5 (>=5.2.0), libqt5multimedia5 (>=5.2.0), libqt5sql5-sqlite (>=5.2.0)")
+	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5webkit5 (>=5.1.0), libqt5script5 (>=5.2.0), libqt5sql5-sqlite (>=5.2.0)")
 	set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "gstreamer0.10-plugins-base, gstreamer0.10-plugins-good")
 	set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
 	set(CPACK_DEBIAN_PACKAGE_SECTION "web")
@@ -68,7 +69,7 @@
 option(EnableQtwebengine "Enable QtWebEngine backend (requires Qt 5.5)" OFF)
 option(EnableQtwebkit "Enable QtWebKit backend (requires Qt 5.2)" ON)
 
-find_package(Qt5 5.2.0 REQUIRED COMPONENTS Core DBus Gui Multimedia Network PrintSupport Script Widgets XmlPatterns)
+find_package(Qt5 5.2.0 REQUIRED COMPONENTS Core Gui Network PrintSupport Script Widgets XmlPatterns)
 find_package(KF5Sonnet)
 
 if (EnableQtwebengine)
@@ -383,15 +384,6 @@
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
@@ -417,8 +409,6 @@
 	find_library(FRAMEWORK_Foundation Foundation)
 
 	target_link_libraries(otter-browser ${FRAMEWORK_Cocoa} ${FRAMEWORK_Foundation})
-elseif (UNIX)
-	qt5_use_modules(otter-browser DBus)
 endif (WIN32)
 
 qt5_use_modules(otter-browser Core Gui Multimedia Network PrintSupport Script WebKit WebKitWidgets Widgets XmlPatterns)
