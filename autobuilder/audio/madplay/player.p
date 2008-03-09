--- player.c.orig	2008-03-08 21:44:04.000000000 -0800
+++ player.c	2008-03-08 21:46:52.000000000 -0800
@@ -607,6 +607,7 @@
  * NAME:        ucs4->length()
  * DESCRIPTION: return the number of ucs4 chars represented by a ucs4 string
  */
+#if 0
 id3_length_t id3_ucs4_length(id3_ucs4_t const *ucs4)
 {
 	id3_ucs4_t const *ptr = ucs4;
@@ -625,6 +626,7 @@
 {
 	  return id3_ucs4_length(ucs4) + 1;
 }
+#endif
 
 /* NAME:	ucs4_to_wchar_strdup()
  * DESCRIPTION: convert an ucs4 string to a wchar_t string
