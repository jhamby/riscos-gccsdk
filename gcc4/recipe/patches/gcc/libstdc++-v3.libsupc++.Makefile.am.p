Index: libstdc++-v3/libsupc++/Makefile.am
===================================================================
--- libstdc++-v3/libsupc++/Makefile.am	(revision 193302)
+++ libstdc++-v3/libsupc++/Makefile.am	(working copy)
@@ -151,7 +151,7 @@
 	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
 
 # LTCOMPILE is copied from LTCXXCOMPILE below.
-LTCOMPILE = $(LIBTOOL) --tag CC --tag disable-shared $(LIBTOOLFLAGS) --mode=compile \
+LTCOMPILE = $(LIBTOOL) --tag CC $(LIBTOOLFLAGS) --mode=compile \
 	    $(CC) $(DEFS) $(C_INCLUDES) \
 	    $(glibcxx_lt_pic_flag) $(glibcxx_compiler_shared_flag) \
 	    $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
@@ -183,7 +183,7 @@
 # last. (That way, things like -O2 passed down from the toplevel can
 # be overridden by --enable-debug.)
 LTCXXCOMPILE = \
-	$(LIBTOOL) --tag CXX --tag disable-shared \
+	$(LIBTOOL) --tag CXX \
 	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=compile $(CXX) $(TOPLEVEL_INCLUDES) \
 	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
@@ -197,7 +197,7 @@
 # directory to configure libstdc++-v3 to use gcc as the C++
 # compilation driver.
 CXXLINK = \
-	$(LIBTOOL) --tag CXX --tag disable-shared \
+	$(LIBTOOL) --tag CXX \
 	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=link $(CXX) \
 	$(OPT_LDFLAGS) $(SECTION_LDFLAGS) $(AM_CXXFLAGS) $(LTLDFLAGS) -o $@
