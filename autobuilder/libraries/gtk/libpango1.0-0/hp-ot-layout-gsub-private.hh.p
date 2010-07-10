--- pango/opentype/hb-ot-layout-gsub-private.hh.orig	2009-11-26 00:44:17.000000000 +0000
+++ pango/opentype/hb-ot-layout-gsub-private.hh	2009-12-27 12:45:10.000000000 +0000
@@ -66,7 +66,7 @@
 					 * beginning of Substitution table */
   SHORT		deltaGlyphID;		/* Add to original GlyphID to get
 					 * substitute GlyphID */
-};
+} PACKED;
 ASSERT_SIZE (SingleSubstFormat1, 6);
 
 struct SingleSubstFormat2
@@ -109,7 +109,7 @@
   ArrayOf<GlyphID>
 		substitute;		/* Array of substitute
 					 * GlyphIDs--ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (SingleSubstFormat2, 6);
 
 struct SingleSubst
@@ -185,7 +185,7 @@
   private:
   ArrayOf<GlyphID>
 		substitute;		/* String of GlyphIDs to substitute */
-};
+} PACKED;
 ASSERT_SIZE (Sequence, 2);
 
 struct MultipleSubstFormat1
@@ -218,7 +218,7 @@
   OffsetArrayOf<Sequence>
 		sequence;		/* Array of Sequence tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (MultipleSubstFormat1, 6);
 
 struct MultipleSubst
@@ -313,7 +313,7 @@
   OffsetArrayOf<AlternateSet>
 		alternateSet;		/* Array of AlternateSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (AlternateSubstFormat1, 6);
 
 struct AlternateSubst
@@ -427,7 +427,7 @@
 		component;		/* Array of component GlyphIDs--start
 					 * with the second  component--ordered
 					 * in writing direction */
-};
+} PACKED;
 ASSERT_SIZE (Ligature, 4);
 
 struct LigatureSet
@@ -459,7 +459,7 @@
   OffsetArrayOf<Ligature>
 		ligature;		/* Array LigatureSet tables
 					 * ordered by preference */
-};
+} PACKED;
 ASSERT_SIZE (LigatureSet, 2);
 
 struct LigatureSubstFormat1
@@ -495,7 +495,7 @@
   OffsetArrayOf<LigatureSet>
 		ligatureSet;		/* Array LigatureSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (LigatureSubstFormat1, 6);
 
 struct LigatureSubst
@@ -632,7 +632,7 @@
   ArrayOf<GlyphID>
 		substituteX;		/* Array of substitute
 					 * GlyphIDs--ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (ReverseChainSingleSubstFormat1, 10);
 
 struct ReverseChainSingleSubst
@@ -719,7 +719,7 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT			sub_format;
   SingleSubst			single[VAR];
   MultipleSubst			multiple[VAR];
@@ -864,7 +864,7 @@
     OffsetTo<SubstLookupList> &list = CAST(OffsetTo<SubstLookupList>, lookupList, 0);
     return SANITIZE_THIS (list);
   }
-};
+} PACKED;
 ASSERT_SIZE (GSUB, 10);
 
 
