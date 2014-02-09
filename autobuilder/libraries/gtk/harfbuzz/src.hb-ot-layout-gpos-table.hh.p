--- src/hb-ot-layout-gpos-table.hh.orig	2013-11-26 19:13:26.023272086 +0000
+++ src/hb-ot-layout-gpos-table.hh	2014-02-08 14:13:46.538167182 +0000
@@ -207,7 +207,7 @@
 
     return TRACE_RETURN (true);
   }
-};
+} PACKED;
 
 
 struct AnchorFormat1
@@ -230,7 +230,7 @@
   SHORT		yCoordinate;		/* Vertical value--in design units */
   public:
   DEFINE_SIZE_STATIC (6);
-};
+} PACKED;
 
 struct AnchorFormat2
 {
@@ -260,7 +260,7 @@
   USHORT	anchorPoint;		/* Index to glyph contour point */
   public:
   DEFINE_SIZE_STATIC (8);
-};
+} PACKED;
 
 struct AnchorFormat3
 {
@@ -295,7 +295,7 @@
 					 * Anchor table (may be NULL) */
   public:
   DEFINE_SIZE_STATIC (10);
-};
+} PACKED;
 
 struct Anchor
 {
@@ -328,10 +328,10 @@
   AnchorFormat1		format1;
   AnchorFormat2		format2;
   AnchorFormat3		format3;
-  } u;
+  } PACKED u;
   public:
   DEFINE_SIZE_UNION (2, format);
-};
+} PACKED;
 
 
 struct AnchorMatrix
@@ -361,7 +361,7 @@
 					 * from beginning of AnchorMatrix table */
   public:
   DEFINE_SIZE_ARRAY (2, matrix);
-};
+} PACKED;
 
 
 struct MarkRecord
@@ -380,7 +380,7 @@
 					 * beginning of MarkArray table */
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 struct MarkArray : ArrayOf<MarkRecord>	/* Array of MarkRecords--in Coverage order */
 {
@@ -419,7 +419,7 @@
     TRACE_SANITIZE (this);
     return TRACE_RETURN (ArrayOf<MarkRecord>::sanitize (c, this));
   }
-};
+} PACKED;
 
 
 /* Lookups */
@@ -468,7 +468,7 @@
 					 * the Coverage table */
   public:
   DEFINE_SIZE_ARRAY (6, values);
-};
+} PACKED;
 
 struct SinglePosFormat2
 {
@@ -517,7 +517,7 @@
 					 * values applied to glyphs */
   public:
   DEFINE_SIZE_ARRAY (8, values);
-};
+} PACKED;
 
 struct SinglePos
 {
@@ -547,8 +547,8 @@
   USHORT		format;		/* Format identifier */
   SinglePosFormat1	format1;
   SinglePosFormat2	format2;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 struct PairValueRecord
@@ -563,7 +563,7 @@
 					 * followed by for second glyph */
   public:
   DEFINE_SIZE_ARRAY (2, values);
-};
+} PACKED;
 
 struct PairSet
 {
@@ -623,7 +623,7 @@
     ValueFormat *valueFormats;
     unsigned int len1; /* valueFormats[0].get_len() */
     unsigned int stride; /* 1 + len1 + len2 */
-  };
+  } PACKED;
 
   inline bool sanitize (hb_sanitize_context_t *c, const sanitize_closure_t *closure) {
     TRACE_SANITIZE (this);
@@ -642,7 +642,7 @@
 					 * by GlyphID of the second glyph */
   public:
   DEFINE_SIZE_ARRAY (2, array);
-};
+} PACKED;
 
 struct PairPosFormat1
 {
@@ -706,7 +706,7 @@
 					 * ordered by Coverage Index */
   public:
   DEFINE_SIZE_ARRAY (10, pairSet);
-};
+} PACKED;
 
 struct PairPosFormat2
 {
@@ -809,7 +809,7 @@
 					 * Each entry has value1 and value2 */
   public:
   DEFINE_SIZE_ARRAY (16, values);
-};
+} PACKED;
 
 struct PairPos
 {
@@ -839,8 +839,8 @@
   USHORT		format;		/* Format identifier */
   PairPosFormat1	format1;
   PairPosFormat2	format2;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 struct EntryExitRecord
@@ -863,7 +863,7 @@
 					 * subtable--may be NULL */
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 struct CursivePosFormat1
 {
@@ -976,7 +976,7 @@
 					 * Coverage Index order */
   public:
   DEFINE_SIZE_ARRAY (6, entryExitRecord);
-};
+} PACKED;
 
 struct CursivePos
 {
@@ -1003,8 +1003,8 @@
   union {
   USHORT		format;		/* Format identifier */
   CursivePosFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 typedef AnchorMatrix BaseArray;		/* base-major--
@@ -1075,7 +1075,7 @@
 					 * beginning of MarkBasePos subtable */
   public:
   DEFINE_SIZE_STATIC (12);
-};
+} PACKED;
 
 struct MarkBasePos
 {
@@ -1102,8 +1102,8 @@
   union {
   USHORT		format;		/* Format identifier */
   MarkBasePosFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 typedef AnchorMatrix LigatureAttach;	/* component-major--
@@ -1196,7 +1196,7 @@
 					 * beginning of MarkLigPos subtable */
   public:
   DEFINE_SIZE_STATIC (12);
-};
+} PACKED;
 
 struct MarkLigPos
 {
@@ -1223,8 +1223,8 @@
   union {
   USHORT		format;		/* Format identifier */
   MarkLigPosFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 typedef AnchorMatrix Mark2Array;	/* mark2-major--
@@ -1315,7 +1315,7 @@
 					 * beginning of MarkMarkPos subtable */
   public:
   DEFINE_SIZE_STATIC (12);
-};
+} PACKED;
 
 struct MarkMarkPos
 {
@@ -1342,18 +1342,18 @@
   union {
   USHORT		format;		/* Format identifier */
   MarkMarkPosFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
-struct ContextPos : Context {};
+struct ContextPos : Context {} PACKED;
 
-struct ChainContextPos : ChainContext {};
+struct ChainContextPos : ChainContext {} PACKED;
 
 struct ExtensionPos : Extension<ExtensionPos>
 {
   typedef struct PosLookupSubTable LookupSubTable;
-};
+} PACKED;
 
 
 
@@ -1418,7 +1418,7 @@
   union {
   struct {
     USHORT		sub_format;
-  } header;
+  } PACKED header;
   SinglePos		single;
   PairPos		pair;
   CursivePos		cursive;
@@ -1428,10 +1428,10 @@
   ContextPos		context;
   ChainContextPos	chainContext;
   ExtensionPos		extension;
-  } u;
+  } PACKED u;
   public:
   DEFINE_SIZE_UNION (2, header.sub_format);
-};
+} PACKED;
 
 
 struct PosLookup : Lookup
@@ -1525,7 +1525,7 @@
   }
   public:
   DEFINE_SIZE_STATIC (10);
-};
+} PACKED;
 
 
 static void
