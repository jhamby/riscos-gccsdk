--- src/corelib/io/qurlidna.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/io/qurlidna.cpp	2014-05-27 19:55:16.350522842 +0100
@@ -2034,7 +2034,7 @@
     const QChar *e = src + source->size();
 
     for ( ; out < e; ++out) {
-        ushort uc = out->unicode();
+        ushort uc = RISCOS_DEREF_QCHAR(out->unicode());
         if (uc >= 0x80) {
             break;
         } else if (uc >= 'A' && uc <= 'Z') {
@@ -2115,7 +2115,7 @@
             containsRandALCat = true;
     }
     if (containsRandALCat) {
-        if (containsLCat || (!isBidirectionalRorAL(src[from].unicode())
+        if (containsLCat || (!isBidirectionalRorAL(RISCOS_DEREF_QCHAR(src[from].unicode()))
                              || !isBidirectionalRorAL(e[-1].unicode())))
             source->resize(from); // not allowed, clear the label
     }
