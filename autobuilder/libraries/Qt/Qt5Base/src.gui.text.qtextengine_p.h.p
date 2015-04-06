--- src/gui/text/qtextengine_p.h.orig	2014-05-15 18:12:15.000000000 +0100
+++ src/gui/text/qtextengine_p.h	2014-06-12 20:10:44.249644237 +0100
@@ -164,7 +164,7 @@
     uchar dontPrint     : 1;
     uchar justification : 4;
     uchar reserved      : 2;
-};
+} __attribute__((packed));
 Q_STATIC_ASSERT(sizeof(QGlyphAttributes) == 1);
 
 struct QGlyphLayout
