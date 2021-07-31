--- src/hb-ot-layout-gdef-table.hh.orig	2013-11-20 19:18:31.714614924 +0000
+++ src/hb-ot-layout-gdef-table.hh	2014-02-08 14:10:24.258164561 +0000
@@ -85,7 +85,7 @@
 					 * in Coverage Index order */
   public:
   DEFINE_SIZE_ARRAY (4, attachPoint);
-};
+} PACKED;
 
 /*
  * Ligature Caret Table
@@ -111,7 +111,7 @@
   SHORT		coordinate;		/* X or Y value, in design units */
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 struct CaretValueFormat2
 {
@@ -137,7 +137,7 @@
   USHORT	caretValuePoint;	/* Contour point index on glyph */
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 struct CaretValueFormat3
 {
@@ -164,7 +164,7 @@
 					 * table */
   public:
   DEFINE_SIZE_STATIC (6);
-};
+} PACKED;
 
 struct CaretValue
 {
@@ -195,10 +195,10 @@
   CaretValueFormat1	format1;
   CaretValueFormat2	format2;
   CaretValueFormat3	format3;
-  } u;
+  } PACKED u;
   public:
   DEFINE_SIZE_UNION (2, format);
-};
+} PACKED;
 
 struct LigGlyph
 {
@@ -231,7 +231,7 @@
 					 * --in increasing coordinate order */
   public:
   DEFINE_SIZE_ARRAY (2, carets);
-};
+} PACKED;
 
 struct LigCaretList
 {
@@ -267,7 +267,7 @@
 					 * in Coverage Index order */
   public:
   DEFINE_SIZE_ARRAY (4, ligGlyph);
-};
+} PACKED;
 
 
 struct MarkGlyphSetsFormat1
@@ -287,7 +287,7 @@
 					 * coverage tables */
   public:
   DEFINE_SIZE_ARRAY (4, coverage);
-};
+} PACKED;
 
 struct MarkGlyphSets
 {
@@ -312,10 +312,10 @@
   union {
   USHORT		format;		/* Format identifier */
   MarkGlyphSetsFormat1	format1;
-  } u;
+  } PACKED u;
   public:
   DEFINE_SIZE_UNION (2, format);
-};
+} PACKED;
 
 
 /*
@@ -424,7 +424,7 @@
 					 * in version 00010002. */
   public:
   DEFINE_SIZE_ARRAY (12, markGlyphSetsDef);
-};
+} PACKED;
 
 
 } /* namespace OT */
