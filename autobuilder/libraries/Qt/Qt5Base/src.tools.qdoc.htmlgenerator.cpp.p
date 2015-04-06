--- src/tools/qdoc/htmlgenerator.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/htmlgenerator.cpp	2014-08-17 16:29:51.203989244 +0100
@@ -3403,12 +3403,12 @@
             APPEND("&gt;");
         } else if (ch == QLatin1Char('"')) {
             APPEND("&quot;");
-        } else if ((outputEncoding == "ISO-8859-1" && ch.unicode() > 0x007F)
+        } else if ((outputEncoding == "ISO-8859-1" && RISCOS_DEREF_QCHAR(ch.unicode()) > 0x007F)
                    || (ch == QLatin1Char('*') && i + 1 < n && string.at(i) == QLatin1Char('/'))
                    || (ch == QLatin1Char('.') && i > 2 && string.at(i - 2) == QLatin1Char('.'))) {
             // we escape '*/' and the last dot in 'e.g.' and 'i.e.' for the Javadoc generator
             APPEND("&#x");
-            html += QString::number(ch.unicode(), 16);
+            html += QString::number(RISCOS_DEREF_QCHAR(ch.unicode()), 16);
             html += QLatin1Char(';');
         } else {
             if (!html.isEmpty())
