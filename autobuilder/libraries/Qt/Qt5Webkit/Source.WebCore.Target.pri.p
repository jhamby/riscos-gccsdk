--- Source/WebCore/Target.pri.orig	2015-02-17 04:57:09.000000000 +0000
+++ Source/WebCore/Target.pri	2015-04-16 15:18:10.761589539 +0100
@@ -2993,6 +2993,8 @@
     SOURCES += editing/SmartReplaceICU.cpp
 }
 
+arm*-riscos*-*:SOURCES += platform/text/TextBreakIteratorICU.cpp
+
 enable?(NETSCAPE_PLUGIN_API) {
 
     SOURCES += plugins/npapi.cpp
