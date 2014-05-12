--- glib/gunicollate.c.orig	2014-02-22 15:29:07.000000000 +0000
+++ glib/gunicollate.c	2014-05-08 17:08:34.148137594 +0100
@@ -36,7 +36,7 @@
 #include "gstrfuncs.h"
 #include "gtestutils.h"
 #include "gcharset.h"
-#ifndef __STDC_ISO_10646__
+#if !defined(__STDC_ISO_10646__) || defined(__riscos__)
 #include "gconvert.h"
 #endif
 
@@ -102,7 +102,7 @@
   g_free (str2_utf16);
   g_free (str1_utf16);
 
-#elif defined(__STDC_ISO_10646__)
+#elif defined(__STDC_ISO_10646__) && !defined(__riscos__)
 
   gunichar *str1_norm;
   gunichar *str2_norm;
@@ -379,7 +379,7 @@
   g_return_val_if_fail (str != NULL, NULL);
   result = carbon_collate_key (str, len);
 
-#elif defined(__STDC_ISO_10646__)
+#elif defined(__STDC_ISO_10646__) && !defined(__riscos__)
 
   gsize xfrm_len;
   gunichar *str_norm;
