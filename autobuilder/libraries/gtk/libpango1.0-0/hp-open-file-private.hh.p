--- pango/opentype/hb-open-file-private.hh.orig	2009-11-17 16:35:44.000000000 +0000
+++ pango/opentype/hb-open-file-private.hh	2009-12-21 18:08:18.000000000 +0000
@@ -45,7 +45,8 @@
 struct OffsetTable;
 struct TTCHeader;
 
-typedef struct TableDirectory
+typedef struct TableDirectory OpenTypeTable;
+struct TableDirectory
 {
   inline bool sanitize (SANITIZE_ARG_DEF, const void *base) {
     TRACE_SANITIZE ();
@@ -58,10 +59,10 @@
   ULONG		offset;		/* Offset from beginning of TrueType font
 				 * file. */
   ULONG		length;		/* Length of this table. */
-} OpenTypeTable;
+} PACKED;
 ASSERT_SIZE (TableDirectory, 16);
 
-typedef struct OffsetTable
+typedef struct PACKED OffsetTable
 {
   friend struct OpenTypeFontFile;
   friend struct TTCHeader;
@@ -159,7 +160,7 @@
   LongOffsetLongArrayOf<OffsetTable>
 		table;		/* Array of offsets to the OffsetTable for each font
 				 * from the beginning of the file */
-};
+} PACKED;
 ASSERT_SIZE (TTCHeader, 12);
 
 
@@ -213,7 +214,7 @@
   }
 
   Tag		tag;		/* 4-byte identifier. */
-};
+} PACKED;
 ASSERT_SIZE (OpenTypeFontFile, 4);
 
 
