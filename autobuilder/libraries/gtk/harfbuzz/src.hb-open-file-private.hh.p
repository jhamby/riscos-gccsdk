--- src/hb-open-file-private.hh.orig	2014-07-11 20:02:15.013287516 +0100
+++ src/hb-open-file-private.hh	2018-11-25 15:04:12.679802315 +0000
@@ -65,7 +65,7 @@
   ULONG		length;		/* Length of this table. */
   public:
   DEFINE_SIZE_STATIC (16);
-} OpenTypeTable;
+} PACKED OpenTypeTable;
 
 typedef struct OffsetTable
 {
@@ -116,7 +116,7 @@
   TableRecord	tables[VAR];	/* TableRecord entries. numTables items */
   public:
   DEFINE_SIZE_ARRAY (12, tables);
-} OpenTypeFontFace;
+} PACKED OpenTypeFontFace;
 
 
 /*
@@ -144,7 +144,7 @@
 				 * from the beginning of the file */
   public:
   DEFINE_SIZE_ARRAY (12, table);
-};
+} PACKED;
 
 struct TTCHeader
 {
@@ -185,9 +185,9 @@
   Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
   FixedVersion	version;	/* Version of the TTC Header (1.0 or 2.0),
 				 * 0x00010000u or 0x00020000u */
-  }			header;
+  } PACKED		header;
   TTCHeaderVersion1	version1;
-  } u;
+  } PACKED u;
 };
 
 
@@ -249,10 +249,10 @@
   Tag			tag;		/* 4-byte identifier. */
   OpenTypeFontFace	fontFace;
   TTCHeader		ttcHeader;
-  } u;
+  } PACKED u;
   public:
   DEFINE_SIZE_UNION (4, tag);
-};
+} PACKED;
 
 
 } /* namespace OT */
