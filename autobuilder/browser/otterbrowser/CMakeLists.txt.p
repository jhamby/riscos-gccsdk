--- CMakeLists.txt.orig	2020-08-30 19:11:44.921028758 +0100
+++ CMakeLists.txt	2020-08-30 19:12:08.980545005 +0100
@@ -12,14 +12,15 @@
 
 set(MAJOR_VERSION "0")
 set(MINOR_VERSION "9")
-set(PATCH_VERSION "91")
+set(PATCH_VERSION "97")
 set(WEEKLY_VERSION "" CACHE STRING "")
 
 add_definitions(-DOTTER_VERSION_MAIN="${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
 add_definitions(-DOTTER_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
 
 if ("${WEEKLY_VERSION}" STREQUAL "")
-	add_definitions(-DOTTER_VERSION_CONTEXT="-dev")
+	string(TIMESTAMP DATE_TIMESTAMP %Y%m%d)
+	add_definitions(-DOTTER_VERSION_CONTEXT="-dev 20170922-3")
 	add_definitions(-DOTTER_VERSION_WEEKLY=" ")
 else ("${WEEKLY_VERSION}" STREQUAL "")
 	add_definitions(-DOTTER_VERSION_CONTEXT=" weekly ${WEEKLY_VERSION}")
@@ -41,7 +42,7 @@
 	set(CPACK_PACKAGE_NAME "otter-browser")
 	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
 	set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
-	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libhunspell-1.3-0 (>= 1.3.3), libqt5multimedia5 (>=5.4.0), libqt5qml5 (>=5.4.0), libqt5svg5 (>=5.4.0), libqt5webkit5 (>=5.4.0), libqt5xmlpatterns5 (>=5.4.0)")
+	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libhunspell-1.3-0 (>= 1.3.3), libqt5multimedia5 (>=5.4.0), libqt5svg5 (>=5.4.0), libqt5webkit5 (>=5.4.0), libqt5xmlpatterns5 (>=5.4.0)")
 	set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "gstreamer1.0-plugins-base, gstreamer1.0-plugins-good")
 	set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
 	set(CPACK_DEBIAN_PACKAGE_SECTION "web")
@@ -81,7 +82,7 @@
 option(ENABLE_QTWEBKIT "Enable QtWebKit backend (requires Qt 5.4)" ON)
 option(ENABLE_CRASHREPORTS "Enable built-in crash reporting (only for official builds)" OFF)
 
-find_package(Qt5 5.4.0 REQUIRED COMPONENTS Core DBus Gui Multimedia Network PrintSupport Qml Svg Widgets XmlPatterns)
+find_package(Qt5 5.4.0 REQUIRED COMPONENTS Core Gui Multimedia Network PrintSupport Svg Widgets XmlPatterns)
 find_package(Qt5WebEngineWidgets 5.6.0 QUIET)
 find_package(Qt5WebKitWidgets 5.4.0 QUIET)
 find_package(Hunspell 1.3.0 QUIET)
@@ -116,7 +117,6 @@
 	src/core/LocalListingNetworkReply.cpp
 	src/core/LongTermTimer.cpp
 	src/core/Migrator.cpp
-	src/core/NetworkAutomaticProxy.cpp
 	src/core/NetworkCache.cpp
 	src/core/NetworkManager.cpp
 	src/core/NetworkManagerFactory.cpp
@@ -493,8 +493,6 @@
 elseif (UNIX)
 	set(otter_src
 		${otter_src}
-		src/modules/platforms/freedesktoporg/FreeDesktopOrgPlatformIntegration.cpp
-		src/modules/platforms/freedesktoporg/FreeDesktopOrgPlatformStyle.cpp
 		3rdparty/libmimeapps/ConfigReader.cpp
 		3rdparty/libmimeapps/DesktopEntry.cpp
 		3rdparty/libmimeapps/Index.cpp
@@ -530,14 +528,13 @@
 
 	target_link_libraries(otter-browser Qt5::MacExtras ${FRAMEWORK_Cocoa} ${FRAMEWORK_Foundation})
 elseif (UNIX)
-	target_link_libraries(otter-browser Qt5::DBus)
 
 	if (ENABLE_CRASHREPORTS)
 		target_link_libraries(otter-browser -lpthread)
 	endif (ENABLE_CRASHREPORTS)
 endif (WIN32)
 
-target_link_libraries(otter-browser Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::Network Qt5::PrintSupport Qt5::Qml Qt5::Svg Qt5::Widgets Qt5::XmlPatterns)
+target_link_libraries(otter-browser Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::Network Qt5::PrintSupport Qt5::Svg Qt5::Widgets Qt5::XmlPatterns)
 
 set(XDG_APPS_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/applications CACHE FILEPATH "Install path for .desktop files")
 
