Index: libstdc++-v3/crossconfig.m4
===================================================================
--- libstdc++-v3/crossconfig.m4	(revision 176416)
+++ libstdc++-v3/crossconfig.m4	(working copy)
@@ -196,6 +196,41 @@
     GLIBCXX_CHECK_MATH_SUPPORT
     GLIBCXX_CHECK_STDLIB_SUPPORT
     ;;
+  arm*-*-riscos)
+    SECTION_FLAGS='-ffunction-sections -fdata-sections'
+    AC_SUBST(SECTION_FLAGS)
+
+    # FIXME: finetune/check this list:
+    AC_DEFINE(HAVE_HYPOT)
+
+    # GLIBCXX_CHECK_STDLIB_SUPPORT
+    AC_DEFINE(HAVE_STRTOF)        
+
+    AC_DEFINE(HAVE_ACOSF)
+    AC_DEFINE(HAVE_ASINF)
+    AC_DEFINE(HAVE_ATAN2F)
+    AC_DEFINE(HAVE_ATANF)
+    AC_DEFINE(HAVE_CEILF)
+    AC_DEFINE(HAVE_COSF)
+    AC_DEFINE(HAVE_COSHF)
+    AC_DEFINE(HAVE_EXPF)
+    AC_DEFINE(HAVE_FABSF)
+    AC_DEFINE(HAVE_FLOORF)
+    AC_DEFINE(HAVE_FMODF)
+    AC_DEFINE(HAVE_FREXPF)
+    AC_DEFINE(HAVE_LDEXPF)
+    AC_DEFINE(HAVE_LOG10F)
+    AC_DEFINE(HAVE_LOGF)
+    AC_DEFINE(HAVE_MODFF)
+    AC_DEFINE(HAVE_POWF)
+    AC_DEFINE(HAVE_SINF)
+    AC_DEFINE(HAVE_SINHF)
+    AC_DEFINE(HAVE_SQRTF)
+    AC_DEFINE(HAVE_TANF)
+    AC_DEFINE(HAVE_TANHF)
+
+    AC_DEFINE(HAVE_ICONV)
+    ;;
   *-solaris*)
     case "$target" in
       *-solaris2.8 | *-solaris2.9 | *-solaris2.10)
