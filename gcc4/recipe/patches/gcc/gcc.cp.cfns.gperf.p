Index: gcc/cp/cfns.gperf
===================================================================
--- gcc/cp/cfns.gperf	(revision 259222)
+++ gcc/cp/cfns.gperf	(working copy)
@@ -18,12 +18,26 @@
 <http://www.gnu.org/licenses/>.  */
 #ifdef __GNUC__
 __inline
+#else
+#ifdef __cplusplus
+inline
+#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
+__attribute__ ((__gnu_inline__))
 #endif
-static unsigned int hash (const char *, unsigned int);
+#endif
+#endif
+static unsigned int hash (const char *, GPERF_LEN_TYPE);
 #ifdef __GNUC__
 __inline
+#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
+__attribute__ ((__gnu_inline__))
 #endif
-const char * libc_name_p (const char *, unsigned int);
+#else
+#ifdef __cplusplus
+inline
+#endif
+#endif
+const char * libc_name_p (const char *, GPERF_LEN_TYPE);
 %}
 %%
 # The standard C library functions, for feeding to gperf; the result is used
