--- pango/opentype/hb-ot-layout-gpos-private.hh.orig	2009-11-17 16:35:44.000000000 +0000
+++ pango/opentype/hb-ot-layout-gpos-private.hh	2009-12-20 19:47:29.000000000 +0000
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
@@ -257,13 +257,13 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   AnchorFormat1		format1[];
   AnchorFormat2		format2[];
   AnchorFormat3		format3[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (Anchor, 2);
 
 
@@ -289,7 +289,7 @@
   OffsetTo<Anchor>
 		matrix[];		/* Matrix of offsets to Anchor tables--
 					 * from beginning of AnchorMatrix table */
-};
+} PACKED;
 ASSERT_SIZE (AnchorMatrix, 2);
 
 
@@ -307,7 +307,7 @@
   OffsetTo<Anchor>
 		markAnchor;		/* Offset to Anchor table--from
 					 * beginning of MarkArray table */
-};
+} PACKED;
 ASSERT_SIZE (MarkRecord, 4);
 
 struct MarkArray
@@ -348,7 +348,7 @@
   private:
   ArrayOf<MarkRecord>
 		markRecord;	/* Array of MarkRecords--in Coverage order */
-};
+} PACKED;
 ASSERT_SIZE (MarkArray, 2);
 
 
@@ -388,7 +388,7 @@
   ValueRecord	values;			/* Defines positioning
 					 * value(s)--applied to all glyphs in
 					 * the Coverage table */
-};
+} PACKED;
 ASSERT_SIZE (SinglePosFormat1, 6);
 
 struct SinglePosFormat2
@@ -430,7 +430,7 @@
   USHORT	valueCount;		/* Number of ValueRecords */
   ValueRecord	values;			/* Array of ValueRecords--positioning
 					 * values applied to glyphs */
-};
+} PACKED;
 ASSERT_SIZE (SinglePosFormat2, 8);
 
 struct SinglePos
@@ -459,12 +459,12 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   SinglePosFormat1	format1[];
   SinglePosFormat2	format2[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (SinglePos, 2);
 
 
@@ -478,7 +478,7 @@
 					 * Coverage table */
   ValueRecord	values;			/* Positioning data for the first glyph
 					 * followed by for second glyph */
-};
+} PACKED;
 ASSERT_SIZE (PairValueRecord, 2);
 
 struct PairSet
@@ -497,7 +497,7 @@
   PairValueRecord
 		array[];		/* Array of PairValueRecords--ordered
 					 * by GlyphID of the second glyph */
-};
+} PACKED;
 ASSERT_SIZE (PairSet, 2);
 
 struct PairPosFormat1
@@ -570,7 +570,7 @@
   OffsetArrayOf<PairSet>
 		pairSet;		/* Array of PairSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (PairPosFormat1, 10);
 
 struct PairPosFormat2
@@ -653,7 +653,7 @@
   ValueRecord	values;			/* Matrix of value pairs:
 					 * class1-major, class2-minor,
 					 * Each entry has value1 and value2 */
-};
+} PACKED;
 ASSERT_SIZE (PairPosFormat2, 16);
 
 struct PairPos
@@ -682,12 +682,12 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   PairPosFormat1	format1[];
   PairPosFormat2	format2[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (PairPos, 2);
 
 
@@ -706,7 +706,7 @@
 		exitAnchor;		/* Offset to ExitAnchor table--from
 					 * beginning of CursivePos
 					 * subtable--may be NULL */
-};
+} PACKED;
 ASSERT_SIZE (EntryExitRecord, 4);
 
 struct CursivePosFormat1
@@ -902,7 +902,7 @@
   ArrayOf<EntryExitRecord>
 		entryExitRecord;	/* Array of EntryExit records--in
 					 * Coverage Index order */
-};
+} PACKED;
 ASSERT_SIZE (CursivePosFormat1, 6);
 
 struct CursivePos
@@ -929,11 +929,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   CursivePosFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (CursivePos, 2);
 
 
@@ -998,7 +998,7 @@
   OffsetTo<BaseArray>
 		baseArray;		/* Offset to BaseArray table--from
 					 * beginning of MarkBasePos subtable */
-};
+} PACKED;
 ASSERT_SIZE (MarkBasePosFormat1, 12);
 
 struct MarkBasePos
@@ -1025,11 +1025,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   MarkBasePosFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (MarkBasePos, 2);
 
 
@@ -1123,7 +1123,7 @@
   OffsetTo<LigatureArray>
 		ligatureArray;		/* Offset to LigatureArray table--from
 					 * beginning of MarkLigPos subtable */
-};
+} PACKED;
 ASSERT_SIZE (MarkLigPosFormat1, 12);
 
 struct MarkLigPos
@@ -1150,11 +1150,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   MarkLigPosFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (MarkLigPos, 2);
 
 
@@ -1224,7 +1224,7 @@
   OffsetTo<Mark2Array>
 		mark2Array;		/* Offset to Mark2Array table--from
 					 * beginning of MarkMarkPos subtable */
-};
+} PACKED;
 ASSERT_SIZE (MarkMarkPosFormat1, 12);
 
 struct MarkMarkPos
@@ -1251,11 +1251,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   MarkMarkPosFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (MarkMarkPos, 2);
 
 
@@ -1271,7 +1271,7 @@
     TRACE_APPLY ();
     return Context::apply (APPLY_ARG, position_lookup);
   }
-};
+} PACKED;
 ASSERT_SIZE (ContextPos, 2);
 
 struct ChainContextPos : ChainContext
@@ -1284,7 +1284,7 @@
     TRACE_APPLY ();
     return ChainContext::apply (APPLY_ARG, position_lookup);
   }
-};
+} PACKED;
 ASSERT_SIZE (ChainContextPos, 2);
 
 
@@ -1299,7 +1299,7 @@
   inline bool apply (APPLY_ARG_DEF) const;
 
   inline bool sanitize (SANITIZE_ARG_DEF);
-};
+} PACKED;
 ASSERT_SIZE (ExtensionPos, 2);
 
 
@@ -1360,7 +1360,7 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;
   SinglePos		single[];
   PairPos		pair[];
@@ -1372,7 +1372,7 @@
   ChainContextPos	chainContext[];
   ExtensionPos		extension[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (PosLookupSubTable, 2);
 
 
@@ -1461,7 +1461,7 @@
     OffsetArrayOf<PosLookupSubTable> &list = (OffsetArrayOf<PosLookupSubTable> &) subTable;
     return SANITIZE_THIS (list);
   }
-};
+} PACKED;
 ASSERT_SIZE (PosLookup, 6);
 
 typedef OffsetListOf<PosLookup> PosLookupList;
@@ -1493,7 +1493,7 @@
     OffsetTo<PosLookupList> &list = CAST(OffsetTo<PosLookupList>, lookupList, 0);
     return SANITIZE_THIS (list);
   }
-};
+} PACKED;
 ASSERT_SIZE (GPOS, 10);
 
 
