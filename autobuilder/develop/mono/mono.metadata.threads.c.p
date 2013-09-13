--- mono/metadata/threads.c.orig	2013-07-30 02:50:25.000000000 +0100
+++ mono/metadata/threads.c	2013-08-02 20:24:44.851899124 +0100
@@ -831,6 +831,16 @@
 void
 mono_thread_get_stack_bounds (guint8 **staddr, size_t *stsize)
 {
+#if __riscos__
+	/* None of what's here really fits with what UnixLib offers, so rather than try to figure
+	 * out the mess below (as somebody else describes), I'll just write my own version.
+	 */
+	pthread_attr_t attr;
+	pthread_attr_init (&attr);
+	pthread_attr_getstackaddr (&attr, staddr);
+	pthread_attr_getstacksize (&attr, stsize);
+	pthread_attr_destroy (&attr);
+#else
 #if defined(HAVE_PTHREAD_GET_STACKSIZE_NP) && defined(HAVE_PTHREAD_GET_STACKADDR_NP)
 	*staddr = (guint8*)pthread_get_stackaddr_np (pthread_self ());
 	*stsize = pthread_get_stacksize_np (pthread_self ());
@@ -897,6 +907,7 @@
 
 	/* When running under emacs, sometimes staddr is not aligned to a page size */
 	*staddr = (guint8*)((gssize)*staddr & ~(mono_pagesize () - 1));
+#endif
 }
 
 MonoThread *
