--- pango/opentype/hb-ot-layout-gpos-private.hh.orig	2009-11-26 00:44:17.000000000 +0000
+++ pango/opentype/hb-ot-layout-gpos-private.hh	2009-12-27 12:39:49.000000000 +0000
@@ -140,7 +140,7 @@
         values++;
     }
   }
-};
+} PACKED;
 ASSERT_SIZE (ValueFormat, 2);
 
 
@@ -165,7 +165,7 @@
   USHORT	format;			/* Format identifier--format = 1 */
   SHORT		xCoordinate;		/* Horizontal value--in design units */
   SHORT		yCoordinate;		/* Vertical value--in design units */
-};
+} PACKED;
 ASSERT_SIZE (AnchorFormat1, 6);
 
 struct AnchorFormat2
@@ -191,7 +191,7 @@
   SHORT		xCoordinate;		/* Horizontal value--in design units */
   SHORT		yCoordinate;		/* Vertical value--in design units */
   USHORT	anchorPoint;		/* Index to glyph contour point */
-};
+} PACKED;
 ASSERT_SIZE (AnchorFormat2, 8);
 
 struct AnchorFormat3
@@ -228,7 +228,7 @@
 		yDeviceTable;		/* Offset to Device table for Y
 					 * coordinate-- from beginning of
 					 * Anchor table (may be NULL) */
-};
+} PACKED;
 ASSERT_SIZE (AnchorFormat3, 10);
 
 struct Anchor
@@ -288,7 +288,7 @@
   OffsetTo<Anchor>
 		matrix[VAR];		/* Matrix of offsets to Anchor tables--
 					 * from beginning of AnchorMatrix table */
-};
+} PACKED;
 ASSERT_SIZE_VAR (AnchorMatrix, 2, OffsetTo<Anchor>);
 
 
@@ -306,7 +306,7 @@
   OffsetTo<Anchor>
 		markAnchor;		/* Offset to Anchor table--from
 					 * beginning of MarkArray table */
-};
+} PACKED;
 ASSERT_SIZE (MarkRecord, 4);
 
 struct MarkArray
@@ -347,7 +347,7 @@
   private:
   ArrayOf<MarkRecord>
 		markRecord;	/* Array of MarkRecords--in Coverage order */
-};
+} PACKED;
 ASSERT_SIZE (MarkArray, 2);
 
 
@@ -387,7 +387,7 @@
   ValueRecord	values;			/* Defines positioning
 					 * value(s)--applied to all glyphs in
 					 * the Coverage table */
-};
+} PACKED;
 ASSERT_SIZE_VAR (SinglePosFormat1, 6, ValueRecord);
 
 struct SinglePosFormat2
@@ -429,7 +429,7 @@
   USHORT	valueCount;		/* Number of ValueRecords */
   ValueRecord	values;			/* Array of ValueRecords--positioning
 					 * values applied to glyphs */
-};
+} PACKED;
 ASSERT_SIZE_VAR (SinglePosFormat2, 8, ValueRecord);
 
 struct SinglePos
@@ -476,7 +476,7 @@
 					 * Coverage table */
   ValueRecord	values;			/* Positioning data for the first glyph
 					 * followed by for second glyph */
-};
+} PACKED;
 ASSERT_SIZE_VAR (PairValueRecord, 2, ValueRecord);
 
 struct PairSet
@@ -495,7 +495,7 @@
   PairValueRecord
 		array[VAR];		/* Array of PairValueRecords--ordered
 					 * by GlyphID of the second glyph */
-};
+} PACKED;
 ASSERT_SIZE_VAR (PairSet, 2, PairValueRecord);
 
 struct PairPosFormat1
@@ -568,7 +568,7 @@
   OffsetArrayOf<PairSet>
 		pairSet;		/* Array of PairSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (PairPosFormat1, 10);
 
 struct PairPosFormat2
@@ -651,7 +651,7 @@
   ValueRecord	values;			/* Matrix of value pairs:
 					 * class1-major, class2-minor,
 					 * Each entry has value1 and value2 */
-};
+} PACKED;
 ASSERT_SIZE_VAR (PairPosFormat2, 16, ValueRecord);
 
 struct PairPos
@@ -703,7 +703,7 @@
 		exitAnchor;		/* Offset to ExitAnchor table--from
 					 * beginning of CursivePos
 					 * subtable--may be NULL */
-};
+} PACKED;
 ASSERT_SIZE (EntryExitRecord, 4);
 
 struct CursivePosFormat1
@@ -899,7 +899,7 @@
   ArrayOf<EntryExitRecord>
 		entryExitRecord;	/* Array of EntryExit records--in
 					 * Coverage Index order */
-};
+} PACKED;
 ASSERT_SIZE (CursivePosFormat1, 6);
 
 struct CursivePos
@@ -993,7 +993,7 @@
   OffsetTo<BaseArray>
 		baseArray;		/* Offset to BaseArray table--from
 					 * beginning of MarkBasePos subtable */
-};
+} PACKED;
 ASSERT_SIZE (MarkBasePosFormat1, 12);
 
 struct MarkBasePos
@@ -1115,7 +1115,7 @@
   OffsetTo<LigatureArray>
 		ligatureArray;		/* Offset to LigatureArray table--from
 					 * beginning of MarkLigPos subtable */
-};
+} PACKED;
 ASSERT_SIZE (MarkLigPosFormat1, 12);
 
 struct MarkLigPos
@@ -1215,7 +1215,7 @@
   OffsetTo<Mark2Array>
 		mark2Array;		/* Offset to Mark2Array table--from
 					 * beginning of MarkMarkPos subtable */
-};
+} PACKED;
 ASSERT_SIZE (MarkMarkPosFormat1, 12);
 
 struct MarkMarkPos
@@ -1347,7 +1347,7 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;
   SinglePos		single[VAR];
   PairPos		pair[VAR];
@@ -1478,7 +1478,7 @@
     OffsetTo<PosLookupList> &list = CAST(OffsetTo<PosLookupList>, lookupList, 0);
     return SANITIZE_THIS (list);
   }
-};
+} PACKED;
 ASSERT_SIZE (GPOS, 10);
 
 
