--- src/platformsupport/fontdatabases/basic/qbasicfontdatabase.cpp.orig	2015-02-17 04:56:38.000000000 +0000
+++ src/platformsupport/fontdatabases/basic/qbasicfontdatabase.cpp	2015-04-12 13:43:51.632657032 +0100
@@ -59,7 +59,7 @@
     quint16 searchRange;
     quint16 entrySelector;
     quint16 rangeShift;
-} OFFSET_TABLE;
+} __attribute__((packed)) OFFSET_TABLE;
 
 typedef struct {
     quint32 tag;
@@ -72,7 +72,7 @@
     quint16 fontSelector;
     quint16 nrCount;
     quint16 storageOffset;
-} NAME_TABLE_HEADER;
+} __attribute__((packed)) NAME_TABLE_HEADER;
 
 typedef struct {
     quint16 platformID;
@@ -81,7 +81,7 @@
     quint16 nameID;
     quint16 stringLength;
     quint16 stringOffset;
-} NAME_RECORD;
+} __attribute__((packed)) NAME_RECORD;
 
 void QBasicFontDatabase::populateFontDatabase()
 {
