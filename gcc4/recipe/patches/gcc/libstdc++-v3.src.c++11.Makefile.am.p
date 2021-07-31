Index: libstdc++-v3/src/c++11/Makefile.am
===================================================================
--- libstdc++-v3/src/c++11/Makefile.am	(revision 193302)
+++ libstdc++-v3/src/c++11/Makefile.am	(working copy)
@@ -102,7 +102,7 @@
 # last. (That way, things like -O2 passed down from the toplevel can
 # be overridden by --enable-debug.)
 LTCXXCOMPILE = \
-	$(LIBTOOL) --tag CXX --tag disable-shared \
+	$(LIBTOOL) --tag CXX \
 	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=compile $(CXX) $(TOPLEVEL_INCLUDES) \
 	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
@@ -116,7 +116,7 @@
 # directory to configure libstdc++-v3 to use gcc as the C++
 # compilation driver.
 CXXLINK = \
-	$(LIBTOOL) --tag CXX --tag disable-shared \
+	$(LIBTOOL) --tag CXX \
 	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=link $(CXX) \
 	$(OPT_LDFLAGS) $(SECTION_LDFLAGS) $(AM_CXXFLAGS) $(LTLDFLAGS) -o $@
