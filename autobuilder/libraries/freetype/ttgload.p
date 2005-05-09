--- src/truetype/ttgload.c.orig	2005-05-09 22:47:21.000000000 +0100
+++ src/truetype/ttgload.c	2005-05-09 22:47:31.000000000 +0100
@@ -1354,12 +1354,6 @@
       error = TT_Err_Invalid_Outline;
       goto Fail;
     }
-    /* invalid contours count */
-    else 
-    {
-      error = TT_Err_Invalid_Outline;
-      goto Fail;
-    }
       
     /***********************************************************************/
     /***********************************************************************/
