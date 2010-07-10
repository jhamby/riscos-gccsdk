--- pango/opentype/hb-ot-layout-common-private.hh.orig	2009-11-26 00:44:17.000000000 +0000
+++ pango/opentype/hb-ot-layout-common-private.hh	2009-12-27 11:59:07.000000000 +0000
@@ -60,7 +60,7 @@
   OffsetTo<Type>
 		offset;		/* Offset from beginning of object holding
 				 * the Record */
-};
+} PACKED;
 
 template <typename Type>
 struct RecordArrayOf : ArrayOf<Record<Type> > {
@@ -97,7 +97,7 @@
     if (index) *index = NO_INDEX;
     return false;
   }
-};
+} PACKED;
 
 template <typename Type>
 struct RecordListOf : RecordArrayOf<Type>
@@ -109,7 +109,7 @@
     TRACE_SANITIZE ();
     return RecordArrayOf<Type>::sanitize (SANITIZE_ARG, CONST_CHARP(this));
   }
-};
+} PACKED;
 
 
 struct IndexArray : ArrayOf<USHORT>
@@ -131,7 +131,7 @@
     *_count = this->len;
     return !!this->len;
   }
-};
+} PACKED;
 
 
 struct Script;
@@ -168,7 +168,7 @@
 				 * language system--if no required features
 				 * = 0xFFFF */
   IndexArray	featureIndex;	/* Array of indices into the FeatureList */
-};
+} PACKED;
 ASSERT_SIZE (LangSys, 6);
 
 
@@ -252,7 +252,7 @@
     Reserved		= 0x00E0u,
     MarkAttachmentType	= 0xFF00u
   };
-};
+} PACKED;
 ASSERT_SIZE (LookupFlag, 2);
 
 struct LookupSubTable
@@ -264,7 +264,7 @@
 
   private:
   USHORT	format;		/* Subtable format.  Different for GSUB and GPOS */
-};
+} PACKED;
 ASSERT_SIZE (LookupSubTable, 2);
 
 struct Lookup
@@ -302,7 +302,7 @@
   USHORT	markFilteringSetX[VAR];	/* Index (base 0) into GDEF mark glyph sets
 					 * structure. This field is only present if bit
 					 * UseMarkFilteringSet of lookup flags is set. */
-};
+} PACKED;
 ASSERT_SIZE_VAR (Lookup, 6, USHORT);
 
 typedef OffsetListOf<Lookup> LookupList;
@@ -341,7 +341,7 @@
   USHORT	coverageFormat;	/* Format identifier--format = 1 */
   ArrayOf<GlyphID>
 		glyphArray;	/* Array of GlyphIDs--in numerical order */
-};
+} PACKED;
 ASSERT_SIZE (CoverageFormat1, 4);
 
 struct CoverageRangeRecord
@@ -367,7 +367,7 @@
   GlyphID	end;			/* Last GlyphID in the range */
   USHORT	startCoverageIndex;	/* Coverage Index of first GlyphID in
 					 * range */
-};
+} PACKED;
 ASSERT_SIZE (CoverageRangeRecord, 6);
 
 struct CoverageFormat2
@@ -399,7 +399,7 @@
 		rangeRecord;	/* Array of glyph ranges--ordered by
 				 * Start GlyphID. rangeCount entries
 				 * long */
-};
+} PACKED;
 ASSERT_SIZE (CoverageFormat2, 4);
 
 struct Coverage
@@ -459,7 +459,7 @@
   GlyphID	startGlyph;		/* First GlyphID of the classValueArray */
   ArrayOf<USHORT>
 		classValue;		/* Array of Class Values--one per GlyphID */
-};
+} PACKED;
 ASSERT_SIZE (ClassDefFormat1, 6);
 
 struct ClassRangeRecord
@@ -484,7 +484,7 @@
   GlyphID	start;		/* First GlyphID in the range */
   GlyphID	end;		/* Last GlyphID in the range */
   USHORT	classValue;	/* Applied to all glyphs in the range */
-};
+} PACKED;
 ASSERT_SIZE (ClassRangeRecord, 6);
 
 struct ClassDefFormat2
@@ -514,7 +514,7 @@
   ArrayOf<ClassRangeRecord>
 		rangeRecord;	/* Array of glyph ranges--ordered by
 				 * Start GlyphID */
-};
+} PACKED;
 ASSERT_SIZE (ClassDefFormat2, 4);
 
 struct ClassDef
@@ -597,7 +597,7 @@
   USHORT	endSize;	/* Largest size to correct--in ppem */
   USHORT	deltaFormat;	/* Format of DeltaValue array data: 1, 2, or 3 */
   USHORT	deltaValue[VAR];	/* Array of compressed data */
-};
+} PACKED;
 ASSERT_SIZE_VAR (Device, 6, USHORT);
 
 
