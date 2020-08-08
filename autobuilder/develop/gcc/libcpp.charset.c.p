--- libcpp/charset.c.orig	2020-07-23 07:35:18.712399623 +0100
+++ libcpp/charset.c	2020-07-24 17:16:30.869914255 +0100
@@ -2136,7 +2136,7 @@
 uchar * 
 _cpp_convert_input (cpp_reader *pfile, const char *input_charset,
 		    uchar *input, size_t size, size_t len,
-		    const unsigned char **buffer_start, off_t *st_size)
+		    const unsigned char **buffer_start, __off_t *st_size)
 {
   struct cset_converter input_cset;
   struct _cpp_strbuf to;
