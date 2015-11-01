Index: gcc/cp/cfns.gperf
===================================================================
--- gcc/cp/cfns.gperf	(revision 229481)
+++ gcc/cp/cfns.gperf	(working copy)
@@ -18,11 +18,25 @@
 <http://www.gnu.org/licenses/>.  */
 #ifdef __GNUC__
 __inline
+#else
+#ifdef __cplusplus
+inline
+#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
+__attribute__ ((__gnu_inline__))
 #endif
+#endif
+#endif
 static unsigned int hash (const char *, unsigned int);
 #ifdef __GNUC__
 __inline
+#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
+__attribute__ ((__gnu_inline__))
 #endif
+#else
+#ifdef __cplusplus
+inline
+#endif
+#endif
 const char * libc_name_p (const char *, unsigned int);
 %}
 %%
