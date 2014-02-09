--- src/hb-ot-hhea-table.hh.orig	2013-11-20 19:18:31.710614931 +0000
+++ src/hb-ot-hhea-table.hh	2014-02-08 14:06:06.382161219 +0000
@@ -88,7 +88,7 @@
 					 * table */
   public:
   DEFINE_SIZE_STATIC (36);
-};
+} PACKED;
 
 
 } /* namespace OT */
