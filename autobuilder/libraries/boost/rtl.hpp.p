--- boost/iostreams/detail/config/rtl.hpp.orig	2009-10-14 16:20:36.000000000 +0100
+++ boost/iostreams/detail/config/rtl.hpp	2011-02-11 14:36:09.679933200 +0000
@@ -45,10 +45,11 @@
 #  define BOOST_IOSTREAMS_FD_OFFSET  long
 # endif
 #else // Non-windows
-# if defined(_LARGEFILE64_SOURCE) && \
+# if (defined(_LARGEFILE64_SOURCE) && \
          (!defined(_FILE_OFFSET_BITS) || _FILE_OFFSET_BITS != 64) || \
      defined(_AIX) && !defined(_LARGE_FILES) || \
-     defined(BOOST_IOSTREAMS_HAS_LARGE_FILE_EXTENSIONS)
+     defined(BOOST_IOSTREAMS_HAS_LARGE_FILE_EXTENSIONS)     \
+	 ) && !defined(__riscos__)
      /**/
 
     /* Systems with transitional extensions for large file support */
