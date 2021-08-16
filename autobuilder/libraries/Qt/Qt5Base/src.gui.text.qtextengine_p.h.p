--- src/gui/text/qtextengine_p.h.orig	2021-08-15 17:59:49.906981448 -0700
+++ src/gui/text/qtextengine_p.h	2021-08-15 18:00:11.027099015 -0700
@@ -172,7 +172,7 @@
     uchar dontPrint     : 1;
     uchar justification : 4;
     uchar reserved      : 2;
-};
+} __attribute__((packed));
 Q_STATIC_ASSERT(sizeof(QGlyphAttributes) == 1);
 
 struct QGlyphLayout
