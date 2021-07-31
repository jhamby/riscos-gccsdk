--- src/corelib/tools/qunicodetools_p.h.orig	2015-02-17 04:56:49.000000000 +0000
+++ src/corelib/tools/qunicodetools_p.h	2015-04-12 13:43:51.596657031 +0100
@@ -59,7 +59,7 @@
     uchar wordStart        : 1;
     uchar wordEnd          : 1;
     uchar mandatoryBreak   : 1;
-};
+} __attribute__((packed));
 Q_DECLARE_TYPEINFO(QCharAttributes, Q_PRIMITIVE_TYPE);
 
 namespace QUnicodeTools {
