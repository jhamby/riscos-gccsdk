--- pango/opentype/hb-ot-layout-gsubgpos-private.hh.orig	2009-11-17 16:35:44.000000000 +0000
+++ pango/opentype/hb-ot-layout-gsubgpos-private.hh	2009-12-20 19:34:39.000000000 +0000
@@ -204,7 +204,7 @@
 					 * sequence--first glyph = 0 */
   USHORT	lookupListIndex;	/* Lookup to apply to that
 					 * position--zero--based */
-};
+} PACKED;
 ASSERT_SIZE (LookupRecord, 4);
 
 static inline bool apply_lookup (APPLY_ARG_DEF,
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
   LookupRecord	lookupRecordX[];	/* Array of LookupRecords--in
 					 * design order */
-};
+} PACKED;
 ASSERT_SIZE (ContextFormat3, 6);
 
 struct Context
@@ -506,13 +506,13 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   ContextFormat1	format1[];
   ContextFormat2	format2[];
   ContextFormat3	format3[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (Context, 2);
 
 
@@ -606,7 +606,7 @@
   ArrayOf<LookupRecord>
 		lookupX;		/* Array of LookupRecords--in
 					 * design order) */
-};
+} PACKED;
 ASSERT_SIZE (ChainRule, 8);
 
 struct ChainRuleSet
@@ -633,7 +633,7 @@
   OffsetArrayOf<ChainRule>
 		rule;			/* Array of ChainRule tables
 					 * ordered by preference */
-};
+} PACKED;
 ASSERT_SIZE (ChainRuleSet, 2);
 
 struct ChainContextFormat1
@@ -669,7 +669,7 @@
   OffsetArrayOf<ChainRuleSet>
 		ruleSet;		/* Array of ChainRuleSet tables
 					 * ordered by Coverage Index */
-};
+} PACKED;
 ASSERT_SIZE (ChainContextFormat1, 6);
 
 struct ChainContextFormat2
@@ -729,7 +729,7 @@
   OffsetArrayOf<ChainRuleSet>
 		ruleSet;		/* Array of ChainRuleSet tables
 					 * ordered by class */
-};
+} PACKED;
 ASSERT_SIZE (ChainContextFormat2, 12);
 
 struct ChainContextFormat3
@@ -790,7 +790,7 @@
   ArrayOf<LookupRecord>
 		lookupX;		/* Array of LookupRecords--in
 					 * design order) */
-};
+} PACKED;
 ASSERT_SIZE (ChainContextFormat3, 10);
 
 struct ChainContext
@@ -819,13 +819,13 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;	/* Format identifier */
   ChainContextFormat1	format1[];
   ChainContextFormat2	format2[];
   ChainContextFormat3	format3[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (ChainContext, 2);
 
 
@@ -857,7 +857,7 @@
 					 * of lookup type subtable.
 					 * Defined as two shorts to avoid
 					 * alignment requirements. */
-};
+} PACKED;
 ASSERT_SIZE (ExtensionFormat1, 8);
 
 struct Extension
@@ -887,11 +887,11 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   ExtensionFormat1	format1[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (Extension, 2);
 
 
@@ -951,7 +951,7 @@
 		featureList; 	/* FeatureList table */
   OffsetTo<LookupList>
 		lookupList; 	/* LookupList table */
-};
+} PACKED;
 ASSERT_SIZE (GSUBGPOS, 10);
 
 
