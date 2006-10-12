--- ./chunk.h.org	2004-10-19 15:51:21.000000000 +0100
+++ ./chunk.h	2006-10-11 22:22:54.760000000 +0100
@@ -79,7 +79,8 @@
  */
 extern
 char	*_dmalloc_chunk_desc_pnt(char *buf, const int buf_size,
-				 const char *file, const unsigned int line);
+				 const char *file, const unsigned int line,
+				 const char *callers[]);
 
 /*
  * int _dmalloc_chunk_read_info
