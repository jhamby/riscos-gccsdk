--- mono/metadata/mono-endian.h.orig	2011-12-19 21:10:24.000000000 +0000
+++ mono/metadata/mono-endian.h	2012-02-23 19:46:52.000000000 +0000
@@ -52,6 +52,16 @@
 		*(dest) = mf.fval;	\
 	} while (0)
 
+#ifdef __riscos__
+guint64 mono_read_double (const unsigned char *x);
+#define readr8(x,dest)	\
+	do {	\
+		mono_rdouble mf;	\
+		mf.ival = mono_read_double (x);	\
+		MONO_DOUBLE_ASSERT_ENDIANITY (&mf.fval);	\
+		*(dest) = mf.fval;	\
+	} while (0)
+#else
 #define readr8(x,dest)	\
 	do {	\
 		mono_rdouble mf;	\
@@ -59,5 +69,6 @@
 		MONO_DOUBLE_ASSERT_ENDIANITY (&mf.fval);	\
 		*(dest) = mf.fval;	\
 	} while (0)
+#endif
 
 #endif /* _MONO_METADATA_ENDIAN_H_ */
