--- libs/system/src/error_code.cpp.orig	2011-02-11 09:18:35.405269200 +0000
+++ libs/system/src/error_code.cpp	2011-02-11 09:19:24.945756800 +0000
@@ -90,7 +90,7 @@
       char buf[64];
       char * bp = buf;
       std::size_t sz = sizeof(buf);
-  #  if defined(__CYGWIN__) || defined(__USE_GNU)
+  #  if defined(__CYGWIN__) || (defined(__USE_GNU) && !defined(__riscos__))
       // Oddball version of strerror_r
       const char * c_str = strerror_r( ev, bp, sz );
       return  c_str
