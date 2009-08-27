--- src/config.c.orig	2009-08-27 08:32:41.000000000 -0700
+++ src/config.c	2009-08-27 08:34:23.000000000 -0700
@@ -53,7 +53,10 @@
 
   /* Read global parameters */
   if ((fp = fopen(parfile, "r")) == NULL) {
-    if (real_uid == 0) {
+#ifndef __riscos__                                                   
+       if (real_uid == 0)                                            
+#endif                                                               
+       {                                                             
       fputs(_("minicom: WARNING: configuration file not found, using defaults\n"),stderr);
       sleep(2);
       return;
