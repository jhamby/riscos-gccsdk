--- src/hb-ot-head-table.hh.orig	2013-11-20 19:18:31.710614931 +0000
+++ src/hb-ot-head-table.hh	2014-02-08 14:05:38.658160860 +0000
@@ -140,7 +140,7 @@
   SHORT		glyphDataFormat;	/* 0 for current format. */
   public:
   DEFINE_SIZE_STATIC (54);
-};
+} PACKED;
 
 
 } /* namespace OT */
