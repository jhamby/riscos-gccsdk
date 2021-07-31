--- player.c.orig	2021-02-04 14:34:12.022277330 +0000
+++ player.c	2021-02-04 14:36:54.811048402 +0000
@@ -607,7 +607,7 @@
  * NAME:        ucs4->length()
  * DESCRIPTION: return the number of ucs4 chars represented by a ucs4 string
  */
-id3_length_t id3_ucs4_length(id3_ucs4_t const *ucs4)
+static id3_length_t id3_ucs4_length(id3_ucs4_t const *ucs4)
 {
 	id3_ucs4_t const *ptr = ucs4;
 
@@ -621,7 +621,7 @@
  * NAME:        ucs4->size()
  * DESCRIPTION: return the encoding size of a ucs4 string
  */
-id3_length_t id3_ucs4_size(id3_ucs4_t const *ucs4)
+static id3_length_t id3_ucs4_size(id3_ucs4_t const *ucs4)
 {
 	  return id3_ucs4_length(ucs4) + 1;
 }
