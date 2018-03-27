--- src/utils.c.orig	2018-01-14 23:22:42.000000000 +1300
+++ src/utils.c	2018-03-27 12:58:02.721094683 +1300
@@ -528,7 +528,7 @@
 touch (const char *file, time_t tm)
 {
 #if HAVE_UTIME
-# ifdef HAVE_STRUCT_UTIMBUF
+# ifdef __riscos__
   struct utimbuf times;
 # else
   struct {
