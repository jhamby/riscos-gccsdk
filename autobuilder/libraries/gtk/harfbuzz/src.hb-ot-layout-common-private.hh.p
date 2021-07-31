--- src/hb-ot-layout-common-private.hh.orig	2013-11-26 23:00:09.816091760 +0000
+++ src/hb-ot-layout-common-private.hh	2014-02-08 14:09:05.910163546 +0000
@@ -64,7 +64,7 @@
   struct sanitize_closure_t {
     hb_tag_t tag;
     void *list_base;
-  };
+  } PACKED;
   inline bool sanitize (hb_sanitize_context_t *c, void *base) {
     TRACE_SANITIZE (this);
     const sanitize_closure_t closure = {tag, base};
@@ -77,7 +77,7 @@
 				 * the Record */
   public:
   DEFINE_SIZE_STATIC (6);
-};
+} PACKED;
 
 template <typename Type>
 struct RecordArrayOf : SortedArrayOf<Record<Type> > {
@@ -112,7 +112,7 @@
       return false;
     }
   }
-};
+} PACKED;
 
 template <typename Type>
 struct RecordListOf : RecordArrayOf<Type>
@@ -124,7 +124,7 @@
     TRACE_SANITIZE (this);
     return TRACE_RETURN (RecordArrayOf<Type>::sanitize (c, this));
   }
-};
+} PACKED;
 
 
 struct RangeRecord
@@ -152,7 +152,7 @@
   USHORT	value;		/* Value */
   public:
   DEFINE_SIZE_STATIC (6);
-};
+} PACKED;
 DEFINE_NULL_DATA (RangeRecord, "\000\001");
 
 
@@ -170,7 +170,7 @@
     }
     return this->len;
   }
-};
+} PACKED;
 
 
 struct Script;
@@ -211,7 +211,7 @@
   IndexArray	featureIndex;	/* Array of indices into the FeatureList */
   public:
   DEFINE_SIZE_ARRAY (6, featureIndex);
-};
+} PACKED;
 DEFINE_NULL_DATA (LangSys, "\0\0\xFF\xFF");
 
 
@@ -251,7 +251,7 @@
 				 * alphabetically by LangSysTag */
   public:
   DEFINE_SIZE_ARRAY (4, langSys);
-};
+} PACKED;
 
 typedef RecordListOf<Script> ScriptList;
 
@@ -365,7 +365,7 @@
 				 * (decipoints). */
   public:
   DEFINE_SIZE_STATIC (10);
-};
+} PACKED;
 
 /* http://www.microsoft.com/typography/otspec/features_pt.htm#ssxx */
 struct FeatureParamsStylisticSet
@@ -398,7 +398,7 @@
 				 * different application interfaces. */
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 /* http://www.microsoft.com/typography/otspec/features_ae.htm#cv01-cv99 */
 struct FeatureParamsCharacterVariants
@@ -439,7 +439,7 @@
 					 * (May be zero.) */
   public:
   DEFINE_SIZE_ARRAY (14, characters);
-};
+} PACKED;
 
 struct FeatureParams
 {
@@ -466,9 +466,9 @@
   FeatureParamsSize			size;
   FeatureParamsStylisticSet		stylisticSet;
   FeatureParamsCharacterVariants	characterVariants;
-  } u;
+  } PACKED u;
   DEFINE_SIZE_STATIC (17);
-};
+} PACKED;
 
 struct Feature
 {
@@ -535,7 +535,7 @@
   IndexArray	 lookupIndex;	/* Array of LookupList indices */
   public:
   DEFINE_SIZE_ARRAY (4, lookupIndex);
-};
+} PACKED;
 
 typedef RecordListOf<Feature> FeatureList;
 
@@ -554,7 +554,7 @@
   };
   public:
   DEFINE_SIZE_STATIC (2);
-};
+} PACKED;
 
 struct Lookup
 {
@@ -615,7 +615,7 @@
 					 * UseMarkFilteringSet of lookup flags is set. */
   public:
   DEFINE_SIZE_ARRAY2 (6, subTable, markFilteringSetX);
-};
+} PACKED;
 
 typedef OffsetListOf<Lookup> LookupList;
 
@@ -687,7 +687,7 @@
 		glyphArray;	/* Array of GlyphIDs--in numerical order */
   public:
   DEFINE_SIZE_ARRAY (4, glyphArray);
-};
+} PACKED;
 
 struct CoverageFormat2
 {
@@ -800,7 +800,7 @@
 				 * long */
   public:
   DEFINE_SIZE_ARRAY (4, rangeRecord);
-};
+} PACKED;
 
 struct Coverage
 {
@@ -920,10 +920,10 @@
   USHORT		format;		/* Format identifier */
   CoverageFormat1	format1;
   CoverageFormat2	format2;
-  } u;
+  } PACKED u;
   public:
   DEFINE_SIZE_UNION (2, format);
-};
+} PACKED;
 
 
 /*
@@ -983,7 +983,7 @@
 		classValue;		/* Array of Class Values--one per GlyphID */
   public:
   DEFINE_SIZE_ARRAY (6, classValue);
-};
+} PACKED;
 
 struct ClassDefFormat2
 {
@@ -1042,7 +1042,7 @@
 				 * Start GlyphID */
   public:
   DEFINE_SIZE_ARRAY (4, rangeRecord);
-};
+} PACKED;
 
 struct ClassDef
 {
@@ -1086,10 +1086,10 @@
   USHORT		format;		/* Format identifier */
   ClassDefFormat1	format1;
   ClassDefFormat2	format2;
-  } u;
+  } PACKED u;
   public:
   DEFINE_SIZE_UNION (2, format);
-};
+} PACKED;
 
 
 /*
@@ -1163,7 +1163,7 @@
   USHORT	deltaValue[VAR];	/* Array of compressed data */
   public:
   DEFINE_SIZE_ARRAY (6, deltaValue);
-};
+} PACKED;
 
 
 } /* namespace OT */
