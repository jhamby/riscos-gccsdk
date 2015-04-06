--- src/corelib/io/qurl.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/io/qurl.cpp	2014-05-27 19:55:15.910522836 +0100
@@ -970,7 +970,7 @@
         // schemes are ASCII only, so we don't need the full Unicode toLower
         QChar *schemeData = scheme.data(); // force detaching here
         for (int i = needsLowercasing; i >= 0; --i) {
-            ushort c = schemeData[i].unicode();
+            ushort c = RISCOS_DEREF_QCHAR(schemeData[i].unicode());
             if (c >= 'A' && c <= 'Z')
                 schemeData[i] = c + 0x20;
         }
@@ -1512,17 +1512,17 @@
         // preceding "/" (if any) from the output buffer;
         if (in <= end - 4 && in[0].unicode() == '/' && in[1].unicode() == '.'
                 && in[2].unicode() == '.' && in[3].unicode() == '/') {
-            while (out > path->constData() && (--out)->unicode() != '/')
+            while (out > path->constData() && RISCOS_DEREF_QCHAR((--out)->unicode()) != '/')
                 ;
-            if (out == path->constData() && out->unicode() != '/')
+            if (out == path->constData() && RISCOS_DEREF_QCHAR(out->unicode()) != '/')
                 ++in;
             in += 3;
             continue;
         } else if (in == end - 3 && in[0].unicode() == '/' && in[1].unicode() == '.'
                    && in[2].unicode() == '.') {
-            while (out > path->constData() && (--out)->unicode() != '/')
+            while (out > path->constData() && RISCOS_DEREF_QCHAR((--out)->unicode()) != '/')
                 ;
-            if (out->unicode() == '/')
+            if (RISCOS_DEREF_QCHAR(out->unicode()) == '/')
                 ++out;
             in += 3;
             break;
