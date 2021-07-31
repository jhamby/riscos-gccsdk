--- configure.in.orig	2013-03-23 16:20:21.271944415 +0000
+++ configure.in	2013-03-23 16:21:04.579823849 +0000
@@ -119,7 +119,7 @@
 if test "x$HAVE_OGG" = "xno"
 then
   dnl fall back to the old school test
-  XIPH_PATH_OGG(, AC_MSG_ERROR(must have Ogg installed!))
+  AC_MSG_ERROR(must have Ogg installed!)
   libs_save=$LIBS
   LIBS="$OGG_LIBS"
   AC_CHECK_FUNC(oggpack_writealign, , AC_MSG_ERROR(Ogg >= 1.0 required !))
