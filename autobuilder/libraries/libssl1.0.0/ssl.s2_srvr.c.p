--- ssl/s2_srvr.c.orig.orig	2015-04-25 19:27:27.000000000 +0100
+++ ssl/s2_srvr.c	2015-04-26 12:14:32.989145652 +0100
@@ -518,7 +518,7 @@
 	/* If a bad decrypt, continue with protocol but with a
 	 * random master secret (Bleichenbacher attack) */
         if ((i < 0) || ((!is_export && i != EVP_CIPHER_key_length(c))
-                    || (is_export && i != ek))) {
+                    || (is_export && i != ek)))
 		{
 		ERR_clear_error();
 		if (is_export)
