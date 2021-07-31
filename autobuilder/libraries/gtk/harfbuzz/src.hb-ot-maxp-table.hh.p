--- src/hb-ot-maxp-table.hh.orig	2013-11-20 19:18:31.722614906 +0000
+++ src/hb-ot-maxp-table.hh	2014-02-08 14:17:57.430170433 +0000
@@ -60,7 +60,7 @@
   USHORT	numGlyphs;		/* The number of glyphs in the font. */
   public:
   DEFINE_SIZE_STATIC (6);
-};
+} PACKED;
 
 
 } /* namespace OT */
