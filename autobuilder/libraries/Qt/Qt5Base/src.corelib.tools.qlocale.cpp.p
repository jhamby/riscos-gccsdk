--- src/corelib/tools/qlocale.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qlocale.cpp	2014-06-11 21:15:13.223531151 +0100
@@ -96,9 +96,9 @@
     int len = code.length();
     if (len != 2 && len != 3)
         return QLocale::C;
-    ushort uc1 = len-- > 0 ? code[0].toLower().unicode() : 0;
-    ushort uc2 = len-- > 0 ? code[1].toLower().unicode() : 0;
-    ushort uc3 = len-- > 0 ? code[2].toLower().unicode() : 0;
+    ushort uc1 = len-- > 0 ? RISCOS_DEREF_QCHAR(code[0].toLower().unicode()) : 0;
+    ushort uc2 = len-- > 0 ? RISCOS_DEREF_QCHAR(code[1].toLower().unicode()) : 0;
+    ushort uc3 = len-- > 0 ? RISCOS_DEREF_QCHAR(code[2].toLower().unicode()) : 0;
 
     const unsigned char *c = language_code_list;
     for (; *c != 0; c += 3) {
@@ -152,9 +152,9 @@
     int len = code.length();
     if (len != 2 && len != 3)
         return QLocale::AnyCountry;
-    ushort uc1 = len-- > 0 ? code[0].toUpper().unicode() : 0;
-    ushort uc2 = len-- > 0 ? code[1].toUpper().unicode() : 0;
-    ushort uc3 = len-- > 0 ? code[2].toUpper().unicode() : 0;
+    ushort uc1 = len-- > 0 ? RISCOS_DEREF_QCHAR(code[0].toUpper().unicode()) : 0;
+    ushort uc2 = len-- > 0 ? RISCOS_DEREF_QCHAR(code[1].toUpper().unicode()) : 0;
+    ushort uc3 = len-- > 0 ? RISCOS_DEREF_QCHAR(code[2].toUpper().unicode()) : 0;
 
     const unsigned char *c = country_code_list;
     for (; *c != 0; c += 3) {
@@ -1562,7 +1562,7 @@
             continue;
         }
 
-        if (format.at(i).toLower().unicode() == 'a')
+        if (RISCOS_DEREF_QCHAR(format.at(i).toLower().unicode()) == 'a')
             return true;
 
         ++i;
@@ -2754,7 +2754,7 @@
             if (digits.length() > 0) {
                 int last_nonzero_idx = digits.length() - 1;
                 while (last_nonzero_idx > 0
-                       && digits.unicode()[last_nonzero_idx] == QLatin1Char('0'))
+                       && RISCOS_DEREF_QCHAR(digits.unicode())[last_nonzero_idx] == QLatin1Char('0'))
                     --last_nonzero_idx;
                 digits.truncate(last_nonzero_idx + 1);
             }
@@ -2907,7 +2907,7 @@
 
     if ((flags & Alternate || flags & ShowBase)
             && base == 8
-            && (num_str.isEmpty() || num_str[0].unicode() != QLatin1Char('0')))
+            && (num_str.isEmpty() || num_str[0].unicode()) != QLatin1Char('0'))
         num_str.prepend(QLatin1Char('0'));
 
     // LeftAdjusted overrides this flag ZeroPadded. sprintf only padds
@@ -2990,7 +2990,7 @@
 
     if ((flags & Alternate || flags & ShowBase)
             && base == 8
-            && (num_str.isEmpty() || num_str[0].unicode() != QLatin1Char('0')))
+            && (num_str.isEmpty() || num_str[0].unicode()) != QLatin1Char('0'))
         num_str.prepend(QLatin1Char('0'));
 
     // LeftAdjusted overrides this flag ZeroPadded. sprintf only padds
