--- mono/metadata/mono-endian.c.orig	2011-12-19 21:10:24.000000000 +0000
+++ mono/metadata/mono-endian.c	2012-02-23 20:06:31.000000000 +0000
@@ -85,4 +85,23 @@
 	return r.i;
 }
 
+#ifdef __riscos__
+guint64 
+mono_read_double (const unsigned char *x)
+{
+	mono_rint64 r;
+
+	r.c [0] = x [4];
+	r.c [1] = x [5];
+	r.c [2] = x [6];
+	r.c [3] = x [7];
+	r.c [4] = x [0];
+	r.c [5] = x [1];
+	r.c [6] = x [2];
+	r.c [7] = x [3];
+
+	return r.i;
+}
+#endif
+
 #endif
