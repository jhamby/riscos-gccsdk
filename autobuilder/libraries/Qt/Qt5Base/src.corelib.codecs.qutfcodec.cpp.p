--- src/corelib/codecs/qutfcodec.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/codecs/qutfcodec.cpp	2014-05-27 19:55:16.358522842 +0100
@@ -486,7 +486,7 @@
                             endian = BigEndianness;
                         } else {
                             endian = LittleEndianness;
-                            ch = QChar((ch.unicode() >> 8) | ((ch.unicode() & 0xff) << 8));
+                            ch = QChar((RISCOS_DEREF_QCHAR(ch.unicode()) >> 8) | ((RISCOS_DEREF_QCHAR(ch.unicode()) & 0xff) << 8));
                         }
                         *qch++ = ch;
                     }
