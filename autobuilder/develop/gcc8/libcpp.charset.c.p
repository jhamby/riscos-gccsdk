--- libcpp/charset.c.orig	2016-02-05 19:39:48.000000000 +0000
+++ libcpp/charset.c	2016-11-16 21:34:17.709039863 +0000
@@ -1714,7 +1714,7 @@
 uchar * 
 _cpp_convert_input (cpp_reader *pfile, const char *input_charset,
 		    uchar *input, size_t size, size_t len,
-		    const unsigned char **buffer_start, off_t *st_size)
+		    const unsigned char **buffer_start, __off_t *st_size)
 {
   struct cset_converter input_cset;
   struct _cpp_strbuf to;
