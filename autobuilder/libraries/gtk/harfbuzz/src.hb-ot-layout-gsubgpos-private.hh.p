--- src/hb-ot-layout-gsubgpos-private.hh.orig	2013-11-20 19:18:31.718614915 +0000
+++ src/hb-ot-layout-gsubgpos-private.hh	2014-02-08 14:17:30.462170084 +0000
@@ -938,7 +938,7 @@
 					 * position--zero--based */
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 
 template <typename context_t>
@@ -1168,7 +1168,7 @@
 					 * design order */
   public:
   DEFINE_SIZE_ARRAY2 (4, input, lookupRecordX);
-};
+} PACKED;
 
 struct RuleSet
 {
@@ -1223,7 +1223,7 @@
 					 * ordered by preference */
   public:
   DEFINE_SIZE_ARRAY (2, rule);
-};
+} PACKED;
 
 
 struct ContextFormat1
@@ -1309,7 +1309,7 @@
 					 * ordered by Coverage Index */
   public:
   DEFINE_SIZE_ARRAY (6, ruleSet);
-};
+} PACKED;
 
 
 struct ContextFormat2
@@ -1404,7 +1404,7 @@
 					 * ordered by class */
   public:
   DEFINE_SIZE_ARRAY (8, ruleSet);
-};
+} PACKED;
 
 
 struct ContextFormat3
@@ -1497,7 +1497,7 @@
 					 * design order */
   public:
   DEFINE_SIZE_ARRAY2 (6, coverage, lookupRecordX);
-};
+} PACKED;
 
 struct Context
 {
@@ -1868,7 +1868,7 @@
 					 * ordered by Coverage Index */
   public:
   DEFINE_SIZE_ARRAY (6, ruleSet);
-};
+} PACKED;
 
 struct ChainContextFormat2
 {
@@ -1992,7 +1992,7 @@
 					 * ordered by class */
   public:
   DEFINE_SIZE_ARRAY (12, ruleSet);
-};
+} PACKED;
 
 struct ChainContextFormat3
 {
@@ -2114,7 +2114,7 @@
 					 * design order) */
   public:
   DEFINE_SIZE_MIN (10);
-};
+} PACKED;
 
 struct ChainContext
 {
@@ -2147,8 +2147,8 @@
   ChainContextFormat1	format1;
   ChainContextFormat2	format2;
   ChainContextFormat3	format3;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 struct ExtensionFormat1
@@ -2170,7 +2170,7 @@
 					 * of lookup type subtable. */
   public:
   DEFINE_SIZE_STATIC (8);
-};
+} PACKED;
 
 template <typename T>
 struct Extension
@@ -2225,8 +2225,8 @@
   union {
   USHORT		format;		/* Format identifier */
   ExtensionFormat1	format1;
-  } u;
-};
+  } PACKED u;
+} PACKED;
 
 
 /*
@@ -2288,7 +2288,7 @@
 		lookupList; 	/* LookupList table */
   public:
   DEFINE_SIZE_STATIC (10);
-};
+} PACKED;
 
 
 } /* namespace OT */
