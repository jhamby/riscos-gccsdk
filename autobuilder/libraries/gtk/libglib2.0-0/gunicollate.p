--- glib/gunicollate.c.orig	2010-01-15 08:21:30.000000000 -0800
+++ glib/gunicollate.c	2010-01-15 08:22:23.000000000 -0800
@@ -37,7 +37,7 @@
 #include "gstrfuncs.h"
 #include "gtestutils.h"
 #include "gcharset.h"
-#ifndef __STDC_ISO_10646__
+#if !defined(__STDC_ISO_10646__) || defined(__riscos__)
 #include "gconvert.h"
 #endif

@@ -95,7 +95,7 @@
   g_free (str2_utf16);
   g_free (str1_utf16);

-#elif defined(__STDC_ISO_10646__)
+#elif defined(__STDC_ISO_10646__) && !defined(__riscos__)

   gunichar *str1_norm;
   gunichar *str2_norm;
@@ -374,7 +374,7 @@
   g_return_val_if_fail (str != NULL, NULL);
   result = carbon_collate_key (str, len);

-#elif defined(__STDC_ISO_10646__)
+#elif defined(__STDC_ISO_10646__) && !defined(__riscos__)

   gsize xfrm_len;
   gunichar *str_norm;
