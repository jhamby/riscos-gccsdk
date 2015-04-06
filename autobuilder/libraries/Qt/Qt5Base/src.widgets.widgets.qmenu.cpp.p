--- src/widgets/widgets/qmenu.cpp.orig	2014-05-15 18:12:12.000000000 +0100
+++ src/widgets/widgets/qmenu.cpp	2014-05-27 19:55:16.370522842 +0100
@@ -2817,7 +2817,7 @@
                     QAction *act = d->actions.at(i);
                     QKeySequence sequence = QKeySequence::mnemonic(act->text());
                     int key = sequence[0] & 0xffff;
-                    if (key == c.unicode()) {
+                    if (key == RISCOS_DEREF_QCHAR(c.unicode())) {
                         clashCount++;
                         if (!first)
                             first = act;
