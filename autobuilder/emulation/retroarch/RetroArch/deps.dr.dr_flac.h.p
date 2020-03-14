--- deps/dr/dr_flac.h.orig	2018-05-03 09:03:22.103242335 +0100
+++ deps/dr/dr_flac.h	2018-05-03 09:03:45.624953080 +0100
@@ -709,6 +709,7 @@
     #endif
 #endif
 
+#ifndef __riscos__
 #if defined(_MSC_VER) && _MSC_VER >= 1300
 #define DRFLAC_HAS_BYTESWAP_INTRINSIC
 #elif defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
@@ -718,6 +719,7 @@
     #define DRFLAC_HAS_BYTESWAP_INTRINSIC
     #endif
 #endif
+#endif
 
 /* Standard library stuff. */
 #ifndef DRFLAC_ASSERT
