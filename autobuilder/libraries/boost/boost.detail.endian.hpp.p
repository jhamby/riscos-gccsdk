--- boost/detail/endian.hpp.orig	2021-02-21 13:12:08.070093448 +0000
+++ boost/detail/endian.hpp	2021-02-21 13:12:52.313381756 +0000
@@ -66,7 +66,8 @@
    || defined(_M_ALPHA) || defined(__amd64) \
    || defined(__amd64__) || defined(_M_AMD64) \
    || defined(__x86_64) || defined(__x86_64__) \
-   || defined(_M_X64) || defined(__bfin__)
+   || defined(_M_X64) || defined(__bfin__) \
+   || defined(__riscos__)
 
 # define BOOST_LITTLE_ENDIAN
 # define BOOST_BYTE_ORDER 1234
