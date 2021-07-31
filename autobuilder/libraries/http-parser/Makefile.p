--- Makefile.orig	2019-04-27 16:03:21.964387000 +0100
+++ Makefile	2019-04-27 16:36:11.293678532 +0100
@@ -95,6 +95,10 @@
 tags: http_parser.c http_parser.h test.c
 	ctags $^
 
+install:
+	cp -f http_parser.h $(GCCSDK_INSTALL_ENV)/include
+	cp -f libhttp_parser.a $(GCCSDK_INSTALL_ENV)/lib
+
 clean:
 	rm -f *.o *.a tags test test_fast test_g \
 		http_parser.tar libhttp_parser.so.* \
