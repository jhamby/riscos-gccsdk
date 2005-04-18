--- src/ltable.c.old	2003-04-03 15:35:34.000000000 +0100
+++ src/ltable.c	2005-03-18 11:57:00.234375000 +0000
@@ -51,8 +51,15 @@
 
 /* function to convert a lua_Number to int (with any rounding method) */
 #ifndef lua_number2int
+
+#ifdef __riscos__
+/* Ranges outside of a the standard integer range cause
+ * an exception on GCC 3.4.4 release 1 on RISC OS */
+#define lua_number2int(i,n) ((i)=(int)(((n) < -2147483648.0) ? -2147483648.0 : ((n) > 2147483647.0) ? 2147483647.0 : (int)(n)))
+#else
 #define lua_number2int(i,n)	((i)=(int)(n))
 #endif
+#endif
 
 
 #define hashpow2(t,n)      (gnode(t, lmod((n), sizenode(t))))
