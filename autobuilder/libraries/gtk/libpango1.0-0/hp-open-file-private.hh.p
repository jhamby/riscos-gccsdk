--- pango/opentype/hb-open-file-private.hh.orig	2009-11-26 00:44:17.000000000 +0000
+++ pango/opentype/hb-open-file-private.hh	2009-12-27 13:19:00.000000000 +0000
@@ -45,7 +45,7 @@
 struct OffsetTable;
 struct TTCHeader;
 
-typedef struct TableDirectory
+typedef struct PACKED TableDirectory
 {
   inline bool sanitize (SANITIZE_ARG_DEF, const void *base) {
     TRACE_SANITIZE ();
@@ -61,7 +61,7 @@
 } OpenTypeTable;
 ASSERT_SIZE (TableDirectory, 16);
 
-typedef struct OffsetTable
+typedef struct PACKED OffsetTable
 {
   friend struct OpenTypeFontFile;
   friend struct TTCHeader;
@@ -159,7 +159,7 @@
   LongOffsetLongArrayOf<OffsetTable>
 		table;		/* Array of offsets to the OffsetTable for each font
 				 * from the beginning of the file */
-};
+} PACKED;
 ASSERT_SIZE (TTCHeader, 12);
 
 
@@ -213,7 +213,7 @@
   }
 
   Tag		tag;		/* 4-byte identifier. */
-};
+} PACKED;
 ASSERT_SIZE (OpenTypeFontFile, 4);
 
 
