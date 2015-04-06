--- src/platformsupport/fontdatabases/basic/qbasicfontdatabase.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/platformsupport/fontdatabases/basic/qbasicfontdatabase.cpp	2014-05-27 19:55:15.910522836 +0100
@@ -67,7 +67,7 @@
     quint16 searchRange;
     quint16 entrySelector;
     quint16 rangeShift;
-} OFFSET_TABLE;
+} __attribute__((packed)) OFFSET_TABLE;
 
 typedef struct {
     quint32 tag;
@@ -80,7 +80,7 @@
     quint16 fontSelector;
     quint16 nrCount;
     quint16 storageOffset;
-} NAME_TABLE_HEADER;
+} __attribute__((packed)) NAME_TABLE_HEADER;
 
 typedef struct {
     quint16 platformID;
@@ -89,7 +89,7 @@
     quint16 nameID;
     quint16 stringLength;
     quint16 stringOffset;
-} NAME_RECORD;
+} __attribute__((packed)) NAME_RECORD;
 
 void QBasicFontDatabase::populateFontDatabase()
 {
