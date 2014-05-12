--- test/Makefile.in.orig	2013-02-10 13:39:16.000000000 +0000
+++ test/Makefile.in	2013-04-16 16:22:44.000000000 +0100
@@ -1284,8 +1284,8 @@
 cairo_test_suite_CFLAGS = $(AM_CFLAGS) $(real_pthread_CFLAGS) 
 cairo_test_suite_LDADD = \
 	$(real_pthread_LIBS)					\
-	$(top_builddir)/test/pdiff/libpdiff.la 			\
-        $(top_builddir)/boilerplate/libcairoboilerplate.la	\
+	$(top_builddir)/test/pdiff/.libs/libpdiff.a-static 			\
+        $(top_builddir)/boilerplate/.libs/libcairoboilerplate.a-static	\
 	$(top_builddir)/src/libcairo.la 			\
 	$(CAIRO_LDADD)
 
