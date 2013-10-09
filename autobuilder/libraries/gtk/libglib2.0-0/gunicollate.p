--- glib/gunicollate.c.orig	2013-05-07 19:26:07.000000000 +0100
+++ glib/gunicollate.c	2013-10-09 12:21:28.189944500 +0100
@@ -37,7 +37,7 @@
 #include "gstrfuncs.h"
 #include "gtestutils.h"
 #include "gcharset.h"
-#ifndef __STDC_ISO_10646__
+#if !defined(__STDC_ISO_10646__) || defined(__riscos__)
 #include "gconvert.h"
 #endif
 
@@ -103,7 +103,7 @@
   g_free (str2_utf16);
   g_free (str1_utf16);
 
-#elif defined(__STDC_ISO_10646__)
+#elif defined(__STDC_ISO_10646__) && !defined(__riscos__)
 
   gunichar *str1_norm;
   gunichar *str2_norm;
@@ -381,7 +381,7 @@
   g_return_val_if_fail (str != NULL, NULL);
   result = carbon_collate_key (str, len);
 
-#elif defined(__STDC_ISO_10646__)
+#elif defined(__STDC_ISO_10646__) && !defined(__riscos__)
 
   gsize xfrm_len;
   gunichar *str_norm;
