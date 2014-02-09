--- src/hb-ot-layout-gsub-table.hh.orig	2013-11-20 19:18:31.718614915 +0000
+++ src/hb-ot-layout-gsub-table.hh	2014-02-08 14:15:51.242168798 +0000
@@ -111,7 +111,7 @@
 					 * substitute GlyphID */
   public:
   DEFINE_SIZE_STATIC (6);
-};
+} PACKED;
 
 struct SingleSubstFormat2
 {
@@ -188,7 +188,7 @@
 					 * GlyphIDs--ordered by Coverage Index */
   public:
   DEFINE_SIZE_ARRAY (6, substitute);
-};
+} PACKED;
 
 struct SingleSubst
 {
@@ -245,8 +245,8 @@
   USHORT		format;		/* Format identifier */
   SingleSubstFormat1	format1;
   SingleSubstFormat2	format2;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 struct Sequence
@@ -311,7 +311,7 @@
 		substitute;		/* String of GlyphIDs to substitute */
   public:
   DEFINE_SIZE_ARRAY (2, substitute);
-};
+} PACKED;
 
 struct MultipleSubstFormat1
 {
@@ -388,7 +388,7 @@
 					 * ordered by Coverage Index */
   public:
   DEFINE_SIZE_ARRAY (6, sequence);
-};
+} PACKED;
 
 struct MultipleSubst
 {
@@ -431,8 +431,8 @@
   union {
   USHORT		format;		/* Format identifier */
   MultipleSubstFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 typedef ArrayOf<GlyphID> AlternateSet;	/* Array of alternate GlyphIDs--in
@@ -539,7 +539,7 @@
 					 * ordered by Coverage Index */
   public:
   DEFINE_SIZE_ARRAY (6, alternateSet);
-};
+} PACKED;
 
 struct AlternateSubst
 {
@@ -582,8 +582,8 @@
   union {
   USHORT		format;		/* Format identifier */
   AlternateSubstFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 struct Ligature
@@ -679,7 +679,7 @@
 					 * in writing direction */
   public:
   DEFINE_SIZE_ARRAY (4, component);
-};
+} PACKED;
 
 struct LigatureSet
 {
@@ -755,7 +755,7 @@
 					 * ordered by preference */
   public:
   DEFINE_SIZE_ARRAY (2, ligature);
-};
+} PACKED;
 
 struct LigatureSubstFormat1
 {
@@ -843,7 +843,7 @@
 					 * ordered by Coverage Index */
   public:
   DEFINE_SIZE_ARRAY (6, ligatureSet);
-};
+} PACKED;
 
 struct LigatureSubst
 {
@@ -889,20 +889,20 @@
   union {
   USHORT		format;		/* Format identifier */
   LigatureSubstFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
-struct ContextSubst : Context {};
+struct ContextSubst : Context {} PACKED;
 
-struct ChainContextSubst : ChainContext {};
+struct ChainContextSubst : ChainContext {} PACKED;
 
 struct ExtensionSubst : Extension<ExtensionSubst>
 {
   typedef struct SubstLookupSubTable LookupSubTable;
 
   inline bool is_reverse (void) const;
-};
+} PACKED;
 
 
 struct ReverseChainSingleSubstFormat1
@@ -1026,7 +1026,7 @@
 					 * GlyphIDs--ordered by Coverage Index */
   public:
   DEFINE_SIZE_MIN (10);
-};
+} PACKED;
 
 struct ReverseChainSingleSubst
 {
@@ -1053,8 +1053,8 @@
   union {
   USHORT				format;		/* Format identifier */
   ReverseChainSingleSubstFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED ;
 
 
 
@@ -1115,7 +1115,7 @@
   union {
   struct {
     USHORT			sub_format;
-  } header;
+  } PACKED header;
   SingleSubst			single;
   MultipleSubst			multiple;
   AlternateSubst		alternate;
@@ -1124,10 +1124,10 @@
   ChainContextSubst		chainContext;
   ExtensionSubst		extension;
   ReverseChainSingleSubst	reverseChainContextSingle;
-  } u;
+  } PACKED u;
   public:
   DEFINE_SIZE_UNION (2, header.sub_format);
-};
+} PACKED;
 
 
 struct SubstLookup : Lookup
@@ -1312,7 +1312,7 @@
   }
   public:
   DEFINE_SIZE_STATIC (10);
-};
+} PACKED;
 
 
 void
