Index: include/system.h
===================================================================
RCS file: /cvs/kaffe/kaffe/include/system.h,v
retrieving revision 1.9
diff -u -r1.9 system.h
--- include/system.h	14 Aug 2001 15:28:13 -0000	1.9
+++ include/system.h	12 Jan 2003 16:51:38 -0000
@@ -40,6 +40,10 @@
 #define	file_separator		"/"
 #define	path_separator		";"
 #define	line_separator		"\n"
+#elif defined(__riscos__)
+#define	file_separator		"/"
+#define	path_separator		","
+#define	line_separator		"\n"
 #else
 #error "Separators undefined for this system"
 #endif
