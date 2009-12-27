--- pango/opentype/hb-ot-layout-gsubgpos-private.hh.orig	2009-11-26 00:44:17.000000000 +0000
+++ pango/opentype/hb-ot-layout-gsubgpos-private.hh	2009-12-27 12:50:00.000000000 +0000
@@ -204,7 +204,7 @@
 					 * sequence--first glyph = 0 */
   USHORT	lookupListIndex;	/* Lookup to apply to that
 					 * position--zero--based */
-};
+} PACKED;
 ASSERT_SIZE (LookupRecord, 4);
 
 static inline bool apply_lookup (APPLY_ARG_DEF,
@@ -322,7 +322,7 @@
 					 * second glyph */
   LookupRecord	lookupRecordX[VAR];	/* Array of LookupRecords--in
 					 * design order */
-};
+} PACKED;
 ASSERT_SIZE_VAR2 (Rule, 4, USHORT, LookupRecord);
 
 struct RuleSet
@@ -385,7 +385,7 @@
   OffsetArrayOf<RuleSet>
 		ruleSet;		/* Array of RuleSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (ContextFormat1, 6);
 
 
@@ -430,7 +430,7 @@
   OffsetArrayOf<RuleSet>
 		ruleSet;		/* Array of RuleSet tables
 					 * ordered by class */
-};
+} PACKED;
 ASSERT_SIZE (ContextFormat2, 8);
 
 
@@ -477,7 +477,7 @@
 					 * table in glyph sequence order */
   LookupRecord	lookupRecordX[VAR];	/* Array of LookupRecords--in
 					 * design order */
-};
+} PACKED;
 ASSERT_SIZE_VAR2 (ContextFormat3, 6, OffsetTo<Coverage>, LookupRecord);
 
 struct Context
@@ -605,7 +605,7 @@
   ArrayOf<LookupRecord>
 		lookupX;		/* Array of LookupRecords--in
 					 * design order) */
-};
+} PACKED;
 ASSERT_SIZE (ChainRule, 8);
 
 struct ChainRuleSet
@@ -632,7 +632,7 @@
   OffsetArrayOf<ChainRule>
 		rule;			/* Array of ChainRule tables
 					 * ordered by preference */
-};
+} PACKED;
 ASSERT_SIZE (ChainRuleSet, 2);
 
 struct ChainContextFormat1
@@ -668,7 +668,7 @@
   OffsetArrayOf<ChainRuleSet>
 		ruleSet;		/* Array of ChainRuleSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (ChainContextFormat1, 6);
 
 struct ChainContextFormat2
@@ -728,7 +728,7 @@
   OffsetArrayOf<ChainRuleSet>
 		ruleSet;		/* Array of ChainRuleSet tables
 					 * ordered by class */
-};
+} PACKED;
 ASSERT_SIZE (ChainContextFormat2, 12);
 
 struct ChainContextFormat3
@@ -789,7 +789,7 @@
   ArrayOf<LookupRecord>
 		lookupX;		/* Array of LookupRecords--in
 					 * design order) */
-};
+} PACKED;
 ASSERT_SIZE (ChainContextFormat3, 10);
 
 struct ChainContext
@@ -855,7 +855,7 @@
 					 * of lookup type subtable.
 					 * Defined as two shorts to avoid
 					 * alignment requirements. */
-};
+} PACKED;
 ASSERT_SIZE (ExtensionFormat1, 8);
 
 struct Extension
@@ -948,7 +948,7 @@
 		featureList; 	/* FeatureList table */
   OffsetTo<LookupList>
 		lookupList; 	/* LookupList table */
-};
+} PACKED;
 ASSERT_SIZE (GSUBGPOS, 10);
 
 
