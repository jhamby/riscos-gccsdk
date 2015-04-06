--- src/corelib/tools/qunicodetools_p.h.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qunicodetools_p.h	2014-06-11 21:20:23.051535166 +0100
@@ -67,7 +67,7 @@
     uchar wordStart        : 1;
     uchar wordEnd          : 1;
     uchar mandatoryBreak   : 1;
-};
+} __attribute__((packed));
 Q_DECLARE_TYPEINFO(QCharAttributes, Q_PRIMITIVE_TYPE);
 
 namespace QUnicodeTools {
