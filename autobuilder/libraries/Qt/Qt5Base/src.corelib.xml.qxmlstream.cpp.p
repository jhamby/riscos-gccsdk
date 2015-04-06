--- src/corelib/xml/qxmlstream.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/xml/qxmlstream.cpp	2014-05-27 19:55:16.246522840 +0100
@@ -1385,7 +1385,7 @@
 
 static inline NameChar fastDetermineNameChar(QChar ch)
 {
-    ushort uc = ch.unicode();
+    ushort uc = RISCOS_DEREF_QCHAR(ch.unicode());
     if (!(uc & ~0x7f)) // uc < 128
         return static_cast<NameChar>(nameCharTable[uc]);
 
@@ -3072,20 +3072,20 @@
     escaped.reserve(s.size());
     for ( int i = 0; i < s.size(); ++i ) {
         QChar c = s.at(i);
-        if (c.unicode() == '<' )
+        if (RISCOS_DEREF_QCHAR(c.unicode()) == '<' )
             escaped.append(QLatin1String("&lt;"));
-        else if (c.unicode() == '>' )
+        else if (RISCOS_DEREF_QCHAR(c.unicode()) == '>' )
             escaped.append(QLatin1String("&gt;"));
-        else if (c.unicode() == '&' )
+        else if (RISCOS_DEREF_QCHAR(c.unicode()) == '&' )
             escaped.append(QLatin1String("&amp;"));
-        else if (c.unicode() == '\"' )
+        else if (RISCOS_DEREF_QCHAR(c.unicode()) == '\"' )
             escaped.append(QLatin1String("&quot;"));
         else if (escapeWhitespace && c.isSpace()) {
-            if (c.unicode() == '\n')
+            if (RISCOS_DEREF_QCHAR(c.unicode()) == '\n')
                 escaped.append(QLatin1String("&#10;"));
-            else if (c.unicode() == '\r')
+            else if (RISCOS_DEREF_QCHAR(c.unicode()) == '\r')
                 escaped.append(QLatin1String("&#13;"));
-            else if (c.unicode() == '\t')
+            else if (RISCOS_DEREF_QCHAR(c.unicode()) == '\t')
                 escaped.append(QLatin1String("&#9;"));
             else
                 escaped += c;
