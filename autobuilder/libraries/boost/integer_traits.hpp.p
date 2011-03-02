--- boost/integer_traits.hpp.orig	2009-12-14 18:14:48.000000000 +0000
+++ boost/integer_traits.hpp	2011-02-11 14:07:01.331252600 +0000
@@ -23,7 +23,7 @@
 #include <limits.h>
 // we need wchar.h for WCHAR_MAX/MIN but not all platforms provide it,
 // and some may have <wchar.h> but not <cwchar> ...
-#if !defined(BOOST_NO_INTRINSIC_WCHAR_T) && (!defined(BOOST_NO_CWCHAR) || defined(sun) || defined(__sun) || defined(__QNX__))
+#if !defined(BOOST_NO_INTRINSIC_WCHAR_T) && (!defined(BOOST_NO_CWCHAR) || defined(sun) || defined(__sun) || defined(__QNX__) || defined(__riscos__))
 #include <wchar.h>
 #endif
 
