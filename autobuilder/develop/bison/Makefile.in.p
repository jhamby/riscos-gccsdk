--- Makefile.in.orig	2015-01-23 12:54:26.000000000 +0000
+++ Makefile.in	2015-03-10 13:01:45.454447662 +0000
@@ -6838,7 +6838,8 @@
 
 clean-local: clean-local-tests
 clean-local-tests:
-	test ! -f $(TESTSUITE) || $(TESTSUITE) -C tests --clean
+	echo "Not cleaning test for RISC OS cross build"
+#test ! -f $(TESTSUITE) || $(TESTSUITE) -C tests --clean
 
 check-local: $(RUN_TESTSUITE_deps)
 	$(RUN_TESTSUITE)
