--- pango/opentype/hb-ot-layout-gsub-private.hh.orig	2009-11-17 16:35:44.000000000 +0000
+++ pango/opentype/hb-ot-layout-gsub-private.hh	2009-12-20 19:42:54.000000000 +0000
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
@@ -139,12 +139,12 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   SingleSubstFormat1	format1[];
   SingleSubstFormat2	format2[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (SingleSubst, 2);
 
 
@@ -186,7 +186,7 @@
   private:
   ArrayOf<GlyphID>
 		substitute;		/* String of GlyphIDs to substitute */
-};
+} PACKED;
 ASSERT_SIZE (Sequence, 2);
 
 struct MultipleSubstFormat1
@@ -219,7 +219,7 @@
   OffsetArrayOf<Sequence>
 		sequence;		/* Array of Sequence tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (MultipleSubstFormat1, 6);
 
 struct MultipleSubst
@@ -247,11 +247,11 @@
   }
 
   private:
-  union {
+  union PACKED{
   USHORT		format;		/* Format identifier */
   MultipleSubstFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (MultipleSubst, 2);
 
 
@@ -315,7 +315,7 @@
   OffsetArrayOf<AlternateSet>
 		alternateSet;		/* Array of AlternateSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (AlternateSubstFormat1, 6);
 
 struct AlternateSubst
@@ -343,11 +343,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   AlternateSubstFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (AlternateSubst, 2);
 
 
@@ -434,7 +434,7 @@
 		component;		/* Array of component GlyphIDs--start
 					 * with the second  component--ordered
 					 * in writing direction */
-};
+} PACKED;
 ASSERT_SIZE (Ligature, 4);
 
 struct LigatureSet
@@ -466,7 +466,7 @@
   OffsetArrayOf<Ligature>
 		ligature;		/* Array LigatureSet tables
 					 * ordered by preference */
-};
+} PACKED;
 ASSERT_SIZE (LigatureSet, 2);
 
 struct LigatureSubstFormat1
@@ -502,7 +502,7 @@
   OffsetArrayOf<LigatureSet>
 		ligatureSet;		/* Array LigatureSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (LigatureSubstFormat1, 6);
 
 struct LigatureSubst
@@ -529,11 +529,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   LigatureSubstFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (LigatureSubst, 2);
 
 
@@ -550,7 +550,7 @@
     TRACE_APPLY ();
     return Context::apply (APPLY_ARG, substitute_lookup);
   }
-};
+} PACKED;
 ASSERT_SIZE (ContextSubst, 2);
 
 struct ChainContextSubst : ChainContext
@@ -563,7 +563,7 @@
     TRACE_APPLY ();
     return ChainContext::apply (APPLY_ARG, substitute_lookup);
   }
-};
+} PACKED;
 ASSERT_SIZE (ChainContextSubst, 2);
 
 
@@ -578,7 +578,7 @@
   inline bool apply (APPLY_ARG_DEF) const;
 
   inline bool sanitize (SANITIZE_ARG_DEF);
-};
+} PACKED;
 ASSERT_SIZE (ExtensionSubst, 2);
 
 
@@ -643,7 +643,7 @@
   ArrayOf<GlyphID>
 		substituteX;		/* Array of substitute
 					 * GlyphIDs--ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (ReverseChainSingleSubstFormat1, 10);
 
 struct ReverseChainSingleSubst
@@ -670,11 +670,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT				format;		/* Format identifier */
   ReverseChainSingleSubstFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (ReverseChainSingleSubst, 2);
 
 
@@ -731,7 +731,7 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT			format;
   SingleSubst			single[];
   MultipleSubst			multiple[];
@@ -742,7 +742,7 @@
   ExtensionSubst		extension[];
   ReverseChainSingleSubst	reverseChainContextSingle[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (SubstLookupSubTable, 2);
 
 
@@ -845,7 +845,7 @@
     OffsetArrayOf<SubstLookupSubTable> &list = (OffsetArrayOf<SubstLookupSubTable> &) subTable;
     return SANITIZE_THIS (list);
   }
-};
+} PACKED;
 ASSERT_SIZE (SubstLookup, 6);
 
 typedef OffsetListOf<SubstLookup> SubstLookupList;
@@ -878,7 +878,7 @@
     OffsetTo<SubstLookupList> &list = CAST(OffsetTo<SubstLookupList>, lookupList, 0);
     return SANITIZE_THIS (list);
   }
-};
+} PACKED;
 ASSERT_SIZE (GSUB, 10);
 
 
