--- glib/gunicollate.c.orig	2019-01-21 13:38:50.000000000 +0000
+++ glib/gunicollate.c	2020-08-02 17:55:12.113956435 +0100
@@ -35,7 +35,7 @@
 #include "gstrfuncs.h"
 #include "gtestutils.h"
 #include "gcharset.h"
-#ifndef __STDC_ISO_10646__
+#if !defined(__STDC_ISO_10646__) || defined(__riscos__)
 #include "gconvert.h"
 #endif
 
@@ -101,7 +101,7 @@
   g_free (str2_utf16);
   g_free (str1_utf16);
 
-#elif defined(__STDC_ISO_10646__)
+#elif defined(__STDC_ISO_10646__) && !defined(__riscos__)
 
   gunichar *str1_norm;
   gunichar *str2_norm;
@@ -382,7 +382,7 @@
   g_return_val_if_fail (str != NULL, NULL);
   result = carbon_collate_key (str, len);
 
-#elif defined(__STDC_ISO_10646__)
+#elif defined(__STDC_ISO_10646__) && !defined(__riscos__)
 
   gsize xfrm_len;
   gunichar *str_norm;
