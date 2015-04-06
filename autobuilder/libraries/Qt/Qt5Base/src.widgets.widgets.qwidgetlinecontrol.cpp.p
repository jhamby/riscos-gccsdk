--- src/widgets/widgets/qwidgetlinecontrol.cpp.orig	2013-08-25 19:03:29.000000000 +0100
+++ src/widgets/widgets/qwidgetlinecontrol.cpp	2013-11-18 15:34:42.552645542 +0000
@@ -976,7 +976,7 @@
         } else if (c == QLatin1Char('!')) {
             m = MaskInputData::NoCaseMode;
         } else if (c != QLatin1Char('{') && c != QLatin1Char('}') && c != QLatin1Char('[') && c != QLatin1Char(']')) {
-            switch (c.unicode()) {
+            switch (RISCOS_DEREF_QCHAR(c.unicode())) {
             case 'A':
             case 'a':
             case 'N':
@@ -1020,7 +1020,7 @@
 */
 bool QWidgetLineControl::isValidInput(QChar key, QChar mask) const
 {
-    switch (mask.unicode()) {
+    switch (RISCOS_DEREF_QCHAR(mask.unicode())) {
     case 'A':
         if (key.isLetter())
             return true;
