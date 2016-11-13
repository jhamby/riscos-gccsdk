--- boost/iostreams/detail/config/rtl.hpp.orig	2016-11-13 11:58:48.777647882 +0000
+++ boost/iostreams/detail/config/rtl.hpp	2016-11-13 12:01:17.761648034 +0000
@@ -45,10 +45,11 @@
 #  define BOOST_IOSTREAMS_FD_OFFSET  long
 # endif
 #else // Non-windows
-# if defined(_LARGEFILE64_SOURCE) && !defined(__APPLE__) && \
+# if (defined(_LARGEFILE64_SOURCE) && !defined(__APPLE__) && \
          (!defined(_FILE_OFFSET_BITS) || _FILE_OFFSET_BITS != 64) || \
      defined(_AIX) && !defined(_LARGE_FILES) || \
-     defined(BOOST_IOSTREAMS_HAS_LARGE_FILE_EXTENSIONS)
+     defined(BOOST_IOSTREAMS_HAS_LARGE_FILE_EXTENSIONS) \
+        ) && !defined(__riscos__)
      /**/
 
     /* Systems with transitional extensions for large file support */
