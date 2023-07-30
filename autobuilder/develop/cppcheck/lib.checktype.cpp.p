--- lib/checktype.cpp.orig	2022-07-16 09:59:15.824216819 +0100
+++ lib/checktype.cpp	2022-07-16 10:08:40.768078945 +0100
@@ -431,9 +431,9 @@
             continue;
         if (!mSettings->isEnabled(&f, false))
             continue;
-        if (f.floatValue >= std::exp2(mSettings->long_long_bit))
+        if (f.floatValue >= exp2(mSettings->long_long_bit))
             floatToIntegerOverflowError(tok, f);
-        else if ((-f.floatValue) > std::exp2(mSettings->long_long_bit - 1))
+        else if ((-f.floatValue) > exp2(mSettings->long_long_bit - 1))
             floatToIntegerOverflowError(tok, f);
         else if (mSettings->platformType != Settings::Unspecified) {
             int bits = 0;
