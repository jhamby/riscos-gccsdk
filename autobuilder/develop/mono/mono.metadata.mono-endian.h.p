--- mono/metadata/mono-endian.h.orig	2013-07-30 02:47:37.000000000 +0100
+++ mono/metadata/mono-endian.h	2013-08-16 13:44:53.644482736 +0100
@@ -39,11 +39,21 @@
 		*(dest) = mf.fval;	\
 	} while (0)
 
+#ifdef __riscos__
+guint64 mono_read_double (const unsigned char *x);
+#define readr8(x,dest)	\
+	do {	\
+		mono_rdouble mf;	\
+		mf.ival = mono_read_double (x);	\
+		*(dest) = mf.fval;	\
+	} while (0)
+#else
 #define readr8(x,dest)	\
 	do {	\
 		mono_rdouble mf;	\
 		mf.ival = read64 ((x));	\
 		*(dest) = mf.fval;	\
 	} while (0)
+#endif
 
 #endif /* _MONO_METADATA_ENDIAN_H_ */
