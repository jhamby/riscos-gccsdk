Index: libstdc++-v3/libsupc++/Makefile.am
===================================================================
--- libstdc++-v3/libsupc++/Makefile.am	(revision 188171)
+++ libstdc++-v3/libsupc++/Makefile.am	(working copy)
@@ -121,7 +121,7 @@
 	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
 
 # LTCOMPILE is copied from LTCXXCOMPILE below.
-LTCOMPILE = $(LIBTOOL) --tag CC --tag disable-shared $(LIBTOOLFLAGS) --mode=compile \
+LTCOMPILE = $(LIBTOOL) --tag CC $(LIBTOOLFLAGS) --mode=compile \
 	    $(CC) $(DEFS) $(C_INCLUDES) $(PIC_CXXFLAGS) \
 	    $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
 
@@ -177,7 +177,7 @@
 # We have to put --tag disable-shared after --tag CXX lest things
 # CXX undo the affect of disable-shared.
 LTCXXCOMPILE = \
-	$(LIBTOOL) --tag CXX --tag disable-shared \
+	$(LIBTOOL) --tag CXX \
 	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=compile $(CXX) $(TOPLEVEL_INCLUDES) \
 	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
@@ -191,7 +191,7 @@
 # directory to configure libstdc++-v3 to use gcc as the C++
 # compilation driver.
 CXXLINK = \
-	$(LIBTOOL) --tag CXX --tag disable-shared \
+	$(LIBTOOL) --tag CXX \
 	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=link $(CXX) \
 	$(OPT_LDFLAGS) $(SECTION_LDFLAGS) $(AM_CXXFLAGS) $(LTLDFLAGS) -o $@
