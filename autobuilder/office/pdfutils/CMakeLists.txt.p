--- CMakeLists.txt.orig	2020-04-25 09:56:43.487801406 +0100
+++ CMakeLists.txt	2020-04-25 09:57:03.455394432 +0100
@@ -187,7 +187,6 @@
   set(ENABLE_GLIB OFF)
 endif()
 if(ENABLE_CPP)
-  macro_optional_find_package(Iconv)
   set(ENABLE_CPP ${ICONV_FOUND})
   set(HAVE_ICONV ${ICONV_FOUND})
 endif()
