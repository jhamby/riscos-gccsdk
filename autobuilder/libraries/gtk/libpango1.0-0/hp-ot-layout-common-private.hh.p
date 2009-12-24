--- pango/opentype/hb-ot-layout-common-private.hh.orig	2009-12-19 12:01:26.000000000 +0000
+++ pango/opentype/hb-ot-layout-common-private.hh	2009-12-20 17:17:12.000000000 +0000
@@ -60,7 +60,7 @@
   OffsetTo<Type>
 		offset;		/* Offset from beginning of object holding
 				 * the Record */
-};
+} PACKED;
 
 template <typename Type>
 struct RecordArrayOf : ArrayOf<Record<Type> > {
@@ -95,7 +95,7 @@
     if (index) *index = NO_INDEX;
     return false;
   }
-};
+} PACKED;
 
 template <typename Type>
 struct RecordListOf : RecordArrayOf<Type>
@@ -107,7 +107,7 @@
     TRACE_SANITIZE ();
     return RecordArrayOf<Type>::sanitize (SANITIZE_ARG, CONST_CHARP(this));
   }
-};
+} PACKED;
 
 
 struct IndexArray : ArrayOf<USHORT>
@@ -128,7 +128,7 @@
     *_count = this->len;
     return !!this->len;
   }
-};
+} PACKED;
 
 
 struct Script;
@@ -165,7 +165,7 @@
 				 * language system--if no required features
 				 * = 0xFFFF */
   IndexArray	featureIndex;	/* Array of indices into the FeatureList */
-};
+} PACKED;
 ASSERT_SIZE_DATA (LangSys, 6, "\0\0\xFF\xFF");
 
 
@@ -249,7 +249,7 @@
     Reserved		= 0x00E0u,
     MarkAttachmentType	= 0xFF00u
   };
-};
+} PACKED;
 ASSERT_SIZE (LookupFlag, 2);
 
 struct LookupSubTable
@@ -261,7 +261,7 @@
 
   private:
   USHORT	format;		/* Subtable format.  Different for GSUB and GPOS */
-};
+} PACKED;
 ASSERT_SIZE (LookupSubTable, 2);
 
 struct Lookup
@@ -299,7 +299,7 @@
   USHORT	markFilteringSetX[0];	/* Index (base 0) into GDEF mark glyph sets
 					 * structure. This field is only present if bit
 					 * UseMarkFilteringSet of lookup flags is set. */
-};
+} PACKED;
 ASSERT_SIZE (Lookup, 6);
 
 typedef OffsetListOf<Lookup> LookupList;
@@ -338,7 +338,7 @@
   USHORT	coverageFormat;	/* Format identifier--format = 1 */
   ArrayOf<GlyphID>
 		glyphArray;	/* Array of GlyphIDs--in numerical order */
-};
+} PACKED;
 ASSERT_SIZE (CoverageFormat1, 4);
 
 struct CoverageRangeRecord
@@ -364,7 +364,7 @@
   GlyphID	end;			/* Last GlyphID in the range */
   USHORT	startCoverageIndex;	/* Coverage Index of first GlyphID in
 					 * range */
-};
+} PACKED;
 ASSERT_SIZE_DATA (CoverageRangeRecord, 6, "\000\001");
 
 struct CoverageFormat2
@@ -396,7 +396,7 @@
 		rangeRecord;	/* Array of glyph ranges--ordered by
 				 * Start GlyphID. rangeCount entries
 				 * long */
-};
+} PACKED;
 ASSERT_SIZE (CoverageFormat2, 4);
 
 struct Coverage
@@ -423,12 +423,12 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   CoverageFormat1	format1[];
   CoverageFormat2	format2[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (Coverage, 2);
 
 
@@ -457,7 +457,7 @@
   GlyphID	startGlyph;		/* First GlyphID of the classValueArray */
   ArrayOf<USHORT>
 		classValue;		/* Array of Class Values--one per GlyphID */
-};
+} PACKED;
 ASSERT_SIZE (ClassDefFormat1, 6);
 
 struct ClassRangeRecord
@@ -482,7 +482,7 @@
   GlyphID	start;		/* First GlyphID in the range */
   GlyphID	end;		/* Last GlyphID in the range */
   USHORT	classValue;	/* Applied to all glyphs in the range */
-};
+} PACKED;
 ASSERT_SIZE_DATA (ClassRangeRecord, 6, "\000\001");
 
 struct ClassDefFormat2
@@ -512,7 +512,7 @@
   ArrayOf<ClassRangeRecord>
 		rangeRecord;	/* Array of glyph ranges--ordered by
 				 * Start GlyphID */
-};
+} PACKED;
 ASSERT_SIZE (ClassDefFormat2, 4);
 
 struct ClassDef
@@ -539,12 +539,12 @@
   }
 
   private:
-  union {
+  union PACKED {
   USHORT		format;		/* Format identifier */
   ClassDefFormat1	format1[];
   ClassDefFormat2	format2[];
   } u;
-};
+} PACKED;
 ASSERT_SIZE (ClassDef, 2);
 
 
@@ -596,7 +596,7 @@
   USHORT	endSize;	/* Largest size to correct--in ppem */
   USHORT	deltaFormat;	/* Format of DeltaValue array data: 1, 2, or 3 */
   USHORT	deltaValue[];	/* Array of compressed data */
-};
+} PACKED;
 ASSERT_SIZE (Device, 6);
 
 
