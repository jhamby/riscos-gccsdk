Index: libgcc/gthr-posix.h
===================================================================
--- libgcc/gthr-posix.h	(revision 217970)
+++ libgcc/gthr-posix.h	(working copy)
@@ -253,11 +253,19 @@
   return __gthread_active_latest_value != 0;
 }
 
-#else /* neither FreeBSD nor Solaris */
+#elif defined(__riscos__) && defined(__TARGET_SCL__) /* neither FreeBSD nor Solaris */
 
 static inline int
 __gthread_active_p (void)
 {
+  return 0;
+}
+
+#else /* not RISC OS with SCL */
+
+static inline int
+__gthread_active_p (void)
+{
 /* Android's C library does not provide pthread_cancel, check for
    `pthread_create' instead.  */
 #ifndef __BIONIC__
