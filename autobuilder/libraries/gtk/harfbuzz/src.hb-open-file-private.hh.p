--- src/hb-open-file-private.hh.orig	2013-03-19 09:47:13.843437293 +0000
+++ src/hb-open-file-private.hh	2014-02-08 14:01:47.890157869 +0000
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
@@ -185,10 +185,10 @@
   Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
   FixedVersion	version;	/* Version of the TTC Header (1.0 or 2.0),
 				 * 0x00010000 or 0x00020000 */
-  }			header;
+  } PACKED	header;
   TTCHeaderVersion1	version1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 /*
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
