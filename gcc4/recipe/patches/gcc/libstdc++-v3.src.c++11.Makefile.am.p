Index: libstdc++-v3/src/c++11/Makefile.am
===================================================================
--- libstdc++-v3/src/c++11/Makefile.am	(revision 188171)
+++ libstdc++-v3/src/c++11/Makefile.am	(working copy)
@@ -96,7 +96,7 @@
 # We have to put --tag disable-shared after --tag CXX lest things
 # CXX undo the affect of disable-shared.
 LTCXXCOMPILE = \
-	$(LIBTOOL) --tag CXX --tag disable-shared \
+	$(LIBTOOL) --tag CXX \
 	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=compile $(CXX) $(TOPLEVEL_INCLUDES) \
 	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
@@ -110,7 +110,7 @@
 # directory to configure libstdc++-v3 to use gcc as the C++
 # compilation driver.
 CXXLINK = \
-	$(LIBTOOL) --tag CXX --tag disable-shared \
+	$(LIBTOOL) --tag CXX \
 	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=link $(CXX) \
 	$(OPT_LDFLAGS) $(SECTION_LDFLAGS) $(AM_CXXFLAGS) $(LTLDFLAGS) -o $@
