--- pango/opentype/hb-ot-layout-gdef-private.hh.orig	2009-11-26 00:44:17.000000000 +0000
+++ pango/opentype/hb-ot-layout-gdef-private.hh	2009-12-27 12:49:36.000000000 +0000
@@ -101,7 +101,7 @@
   private:
   USHORT	caretValueFormat;	/* Format identifier--format = 1 */
   SHORT		coordinate;		/* X or Y value, in design units */
-};
+} PACKED;
 ASSERT_SIZE (CaretValueFormat1, 4);
 
 struct CaretValueFormat2
@@ -122,7 +122,7 @@
   private:
   USHORT	caretValueFormat;	/* Format identifier--format = 2 */
   USHORT	caretValuePoint;	/* Contour point index on glyph */
-};
+} PACKED;
 ASSERT_SIZE (CaretValueFormat2, 4);
 
 struct CaretValueFormat3
@@ -148,7 +148,7 @@
 		deviceTable;		/* Offset to Device table for X or Y
 					 * value--from beginning of CaretValue
 					 * table */
-};
+} PACKED;
 ASSERT_SIZE (CaretValueFormat3, 6);
 
 struct CaretValue
@@ -208,7 +208,7 @@
 		carets;			/* Offset array of CaretValue tables
 					 * --from beginning of LigGlyph table
 					 * --in increasing coordinate order */
-};
+} PACKED;
 ASSERT_SIZE (LigGlyph, 2);
 
 struct LigCaretList
@@ -260,7 +260,7 @@
   LongOffsetArrayOf<Coverage>
 		coverage;		/* Array of long offsets to mark set
 					 * coverage tables */
-};
+} PACKED;
 ASSERT_SIZE (MarkGlyphSetsFormat1, 4);
 
 struct MarkGlyphSets
@@ -366,7 +366,7 @@
 					 * definitions--from beginning of GDEF
 					 * header (may be NULL).  Introduced
 					 * in version 00010002. */
-};
+} PACKED;
 ASSERT_SIZE_VAR (GDEF, 12, OffsetTo<MarkGlyphSets>);
 
 
