--- screen.h.orig	2004-12-22 11:14:45.000000000 +0000
+++ screen.h	2004-12-22 11:14:49.000000000 +0000
@@ -20,8 +20,8 @@
 extern unsigned char start_font2[4096];
 extern unsigned char start_font3[4096];
 
-int monthgraph_style;
-int mps_global_style;
+extern int monthgraph_style;
+extern int mps_global_style;
 
 /* This is on in screen_full_refresh, used in *_refresh() */
 extern char screen_refreshing;
