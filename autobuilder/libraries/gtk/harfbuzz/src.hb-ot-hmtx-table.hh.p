--- src/hb-ot-hmtx-table.hh.orig	2013-11-20 19:18:31.714614924 +0000
+++ src/hb-ot-hmtx-table.hh	2014-02-08 14:06:37.582161623 +0000
@@ -46,7 +46,7 @@
   SHORT		lsb;
   public:
   DEFINE_SIZE_STATIC (4);
-};
+} PACKED;
 
 struct hmtx
 {
@@ -83,7 +83,7 @@
 					 * values for each glyph. */
   public:
   DEFINE_SIZE_ARRAY2 (0, longHorMetric, leftSideBearingX);
-};
+} PACKED;
 
 
 } /* namespace OT */
