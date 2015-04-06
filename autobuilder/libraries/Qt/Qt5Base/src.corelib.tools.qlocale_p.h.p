--- src/corelib/tools/qlocale_p.h.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qlocale_p.h	2014-05-28 21:03:29.695735065 +0100
@@ -369,10 +369,10 @@
 {
     const ushort tenUnicode = m_zero + 10;
 
-    if (in.unicode() >= m_zero && in.unicode() < tenUnicode)
-        return '0' + in.unicode() - m_zero;
+    if (RISCOS_DEREF_QCHAR(in.unicode()) >= m_zero && RISCOS_DEREF_QCHAR(in.unicode()) < tenUnicode)
+        return '0' + RISCOS_DEREF_QCHAR(in.unicode()) - m_zero;
 
-    if (in.unicode() >= '0' && in.unicode() <= '9')
+    if (RISCOS_DEREF_QCHAR(in.unicode()) >= '0' && RISCOS_DEREF_QCHAR(in.unicode()) <= '9')
         return in.toLatin1();
 
     if (in == m_plus || in == QLatin1Char('+'))
@@ -392,7 +392,7 @@
 
     // In several languages group() is the char 0xA0, which looks like a space.
     // People use a regular space instead of it and complain it doesn't work.
-    if (m_group == 0xA0 && in.unicode() == ' ')
+    if (m_group == 0xA0 && RISCOS_DEREF_QCHAR(in.unicode()) == ' ')
         return ',';
 
     return 0;
