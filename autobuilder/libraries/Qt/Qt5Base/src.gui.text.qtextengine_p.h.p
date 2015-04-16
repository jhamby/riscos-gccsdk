--- src/gui/text/qtextengine_p.h.orig	2015-02-17 04:56:47.000000000 +0000
+++ src/gui/text/qtextengine_p.h	2015-04-12 13:43:51.576657030 +0100
@@ -156,7 +156,7 @@
     uchar dontPrint     : 1;
     uchar justification : 4;
     uchar reserved      : 2;
-};
+} __attribute__((packed));
 Q_STATIC_ASSERT(sizeof(QGlyphAttributes) == 1);
 
 struct QGlyphLayout
