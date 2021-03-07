--- datafun.c.orig	2021-03-06 17:48:29.852064064 +0000
+++ datafun.c	2021-03-06 17:50:44.630832669 +0000
@@ -13,7 +13,7 @@
   char buf[4096];
 
   buf[sizeof(buf) - 1] = 0;
-  strcpy(buf, "./data");
+  strcpy(buf, "/<RockDodger$Dir>/data");
   if(missing(buf)) {
     char *env;
 
