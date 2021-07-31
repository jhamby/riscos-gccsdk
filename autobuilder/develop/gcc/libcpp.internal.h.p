--- libcpp/internal.h.orig	2020-07-23 07:35:18.712399623 +0100
+++ libcpp/internal.h	2020-07-24 17:16:30.829914587 +0100
@@ -812,7 +812,7 @@
 extern void _cpp_destroy_iconv (cpp_reader *);
 extern unsigned char *_cpp_convert_input (cpp_reader *, const char *,
 					  unsigned char *, size_t, size_t,
-					  const unsigned char **, off_t *);
+					  const unsigned char **, __off_t *);
 extern const char *_cpp_default_encoding (void);
 extern cpp_hashnode * _cpp_interpret_identifier (cpp_reader *pfile,
 						 const unsigned char *id,
