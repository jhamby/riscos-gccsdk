--- CMakeLists.txt.orig	2016-04-23 10:14:35.107661530 +0100
+++ CMakeLists.txt	2016-04-23 10:18:43.079661784 +0100
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
-	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libhunspell-1.3-0 (>= 1.3.3), libqt5multimedia5 (>=5.2.0), libqt5qml5 (>=5.2.0), libqt5webkit5 (>=5.2.0), libqt5xmlpatterns5 (>=5.2.0)")
+	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libhunspell-1.3-0 (>= 1.3.3), libqt5multimedia5 (>=5.2.0), libqt5webkit5 (>=5.2.0), libqt5xmlpatterns5 (>=5.2.0)")
 	set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "gstreamer0.10-plugins-base, gstreamer0.10-plugins-good")
 	set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
 	set(CPACK_DEBIAN_PACKAGE_SECTION "web")
@@ -69,7 +70,7 @@
 option(ENABLE_QTWEBENGINE "Enable QtWebEngine backend (requires Qt 5.6)" ON)
 option(ENABLE_QTWEBKIT "Enable QtWebKit backend (requires Qt 5.2)" ON)
 
-find_package(Qt5 5.2.0 REQUIRED COMPONENTS Core DBus Gui Multimedia Network PrintSupport Qml Widgets XmlPatterns)
+find_package(Qt5 5.2.0 REQUIRED COMPONENTS Core Gui Multimedia Network PrintSupport Widgets XmlPatterns)
 find_package(Qt5WebEngineWidgets 5.6.0 QUIET)
 find_package(Qt5WebKitWidgets 5.2.0 QUIET)
 find_package(Hunspell 1.3.0 QUIET)
@@ -103,7 +104,6 @@
 	src/core/InputInterpreter.cpp
 	src/core/LocalListingNetworkReply.cpp
 	src/core/LongTermTimer.cpp
-	src/core/NetworkAutomaticProxy.cpp
 	src/core/NetworkCache.cpp
 	src/core/NetworkManager.cpp
 	src/core/NetworkManagerFactory.cpp
@@ -414,7 +414,6 @@
 elseif (UNIX)
 	set(otter_src
 		${otter_src}
-		src/modules/platforms/freedesktoporg/FreeDesktopOrgPlatformIntegration.cpp
 		3rdparty/libmimeapps/ConfigReader.cpp
 		3rdparty/libmimeapps/DesktopEntry.cpp
 		3rdparty/libmimeapps/Index.cpp
@@ -449,11 +448,9 @@
 	find_library(FRAMEWORK_Foundation Foundation)
 
 	target_link_libraries(otter-browser ${FRAMEWORK_Cocoa} ${FRAMEWORK_Foundation})
-elseif (UNIX)
-	qt5_use_modules(otter-browser DBus)
 endif (WIN32)
 
-qt5_use_modules(otter-browser Core Gui Multimedia Network PrintSupport Qml Widgets XmlPatterns)
+qt5_use_modules(otter-browser Core Gui Multimedia Network PrintSupport Widgets XmlPatterns)
 
 set(OTTER_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
 set(XDG_APPS_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/applications CACHE FILEPATH "Install path for .desktop files")
