--- endian.h.orig	2008-08-29 12:58:02.196138400 +0100
+++ endian.h	2008-08-29 13:00:38.877138400 +0100
@@ -52,11 +52,29 @@
 	return s;
 }
 #else
+#ifdef __riscos__
+#define FetchLE16(p) ((unsigned short)( *((unsigned char *)p) \
+                     + ((*((unsigned char *)p+1) << 8) ))); p += 2
+#define FetchLE32(p) ((unsigned int)( *((unsigned char *)p) \
+                   + (*((unsigned char *)p+1) << 8) \
+                   + (*((unsigned char *)p+2) << 16) \
+                   + (*((unsigned char *)p+3) << 24))); p += 4
+#else
 #define FetchLE16(p) (*((unsigned short*)(p))); p += 2
 #define FetchLE32(p) (*((unsigned int*)(p))); p += 4
 #endif
+#endif
+#ifdef __riscos__
+#define AccessLE16(p) ((unsigned short)( *((unsigned char *)p) \
+                     + ((*((unsigned char *)p+1) << 8) )))
+#define AccessLE32(p)  ((unsigned int)( *((unsigned char *)p) \
+                   + (*((unsigned char *)p+1) << 8) \
+                   + (*((unsigned char *)p+2) << 16) \
+                   + (*((unsigned char *)p+3) << 24)))
+#else
 #define AccessLE16(p) (*((unsigned short*)(p)))
 #define AccessLE32(p) (*((unsigned int*)(p)))
+#endif
 #define ConvertLE16(v) (v)
 #define ConvertLE32(v) (v)
 #else
