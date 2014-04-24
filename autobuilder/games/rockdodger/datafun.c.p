--- datafun.c.orig	2013-12-28 16:42:18.000000000 +0000
+++ datafun.c	2014-04-11 12:52:31.000000000 +0100
@@ -17,7 +17,12 @@
   // default: ./data
   // second alternative: ROCK_DODGERS_DATADIR
   // final alternative: @datadir@/@PACKAGENAME@
+#ifdef __riscos__
+    strcpy(buf, "<RockD$Dir>/data");
+    if (missing(data_dir))
+#else
   strcpy(buf, "./data");
+#endif
   if(missing(buf)) {
     char *env;
 
