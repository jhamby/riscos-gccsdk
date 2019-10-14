--- libcpp/internal.h.orig	2016-04-06 19:35:16.000000000 +0100
+++ libcpp/internal.h	2016-11-16 21:34:45.777040226 +0000
@@ -752,7 +752,7 @@
 extern void _cpp_destroy_iconv (cpp_reader *);
 extern unsigned char *_cpp_convert_input (cpp_reader *, const char *,
 					  unsigned char *, size_t, size_t,
-					  const unsigned char **, off_t *);
+					  const unsigned char **, __off_t *);
 extern const char *_cpp_default_encoding (void);
 extern cpp_hashnode * _cpp_interpret_identifier (cpp_reader *pfile,
 						 const unsigned char *id,
