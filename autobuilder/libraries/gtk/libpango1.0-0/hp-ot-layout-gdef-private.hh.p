--- pango/opentype/hb-ot-layout-gdef-private.hh.orig	2009-11-17 16:35:44.000000000 +0000
+++ pango/opentype/hb-ot-layout-gdef-private.hh	2009-12-20 19:28:27.000000000 +0000
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
@@ -175,13 +175,13 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   CaretValueFormat1	format1[];
   CaretValueFormat2	format2[];
   CaretValueFormat3	format3[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (CaretValue, 2);
 
 struct LigGlyph
@@ -209,7 +209,7 @@
 		carets;			/* Offset array of CaretValue tables
 					 * --from beginning of LigGlyph table
 					 * --in increasing coordinate order */
-};
+} PACKED;
 ASSERT_SIZE (LigGlyph, 2);
 
 struct LigCaretList
@@ -261,7 +261,7 @@
   LongOffsetArrayOf<Coverage>
 		coverage;		/* Array of long offsets to mark set
 					 * coverage tables */
-};
+} PACKED;
 ASSERT_SIZE (MarkGlyphSetsFormat1, 4);
 
 struct MarkGlyphSets
@@ -284,11 +284,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   MarkGlyphSetsFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (MarkGlyphSets, 2);
 
 
