--- libstdc++-v3/crossconfig.m4.orig	2006-12-12 15:18:36.000000000 +0100
+++ libstdc++-v3/crossconfig.m4	2010-07-04 19:34:10.416905223 +0200
@@ -304,6 +304,35 @@ case "${host}" in
     AC_DEFINE(HAVE_SINHF)
     AC_DEFINE(HAVE_SINHL)
     ;;
+  arm*-*-riscos*)
+    AC_CHECK_HEADERS([nan.h ieeefp.h endian.h sys/isa_defs.h \
+      machine/endian.h machine/param.h sys/machine.h sys/types.h \
+      fp.h float.h endian.h inttypes.h locale.h float.h stdint.h])
+    SECTION_FLAGS='-ffunction-sections -fdata-sections'
+    AC_SUBST(SECTION_FLAGS)
+    GLIBCXX_CHECK_LINKER_FEATURES
+    GLIBCXX_CHECK_COMPLEX_MATH_SUPPORT
+    GLIBCXX_CHECK_ICONV_SUPPORT
+
+    # For LFS.
+    AC_DEFINE(HAVE_INT64_T)
+
+    # For showmanyc_helper().
+    AC_CHECK_HEADERS(sys/ioctl.h sys/filio.h)
+    GLIBCXX_CHECK_POLL
+    GLIBCXX_CHECK_S_ISREG_OR_S_IFREG
+
+    # For xsputn_2().
+    AC_CHECK_HEADERS(sys/uio.h)
+    GLIBCXX_CHECK_WRITEV
+
+    GLIBCXX_CHECK_MATH_SUPPORT
+    GLIBCXX_CHECK_BUILTIN_MATH_SUPPORT
+    GLIBCXX_CHECK_COMPLEX_MATH_SUPPORT
+    GLIBCXX_CHECK_ICONV_SUPPORT
+    GLIBCXX_CHECK_STDLIB_SUPPORT
+    GLIBCXX_CHECK_S_ISREG_OR_S_IFREG
+    ;;
   *-solaris*)
     case "$target" in
     #  *-solaris2.5)
