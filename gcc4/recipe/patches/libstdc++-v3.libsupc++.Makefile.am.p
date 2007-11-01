--- libstdc++-v3/libsupc++/Makefile.am.orig	2007-10-31 18:12:46.000000000 +0000
+++ libstdc++-v3/libsupc++/Makefile.am	2007-10-31 18:12:03.000000000 +0000
@@ -28,8 +28,7 @@
 # 1) separate libsupc++.la
 toolexeclib_LTLIBRARIES = libsupc++.la
 # 2) integrated libsupc++convenience.la that is to be a part of libstdc++.a
-noinst_LTLIBRARIES = libsupc++convenience.la
-
+#noinst_LTLIBRARIES = libsupc++convenience.la
 
 headers = \
 	exception new typeinfo cxxabi.h exception_defines.h
@@ -70,7 +69,8 @@
 	vterminate.cc
 
 libsupc___la_SOURCES = $(sources) $(c_sources)
-libsupc__convenience_la_SOURCES = $(sources) $(c_sources)
+libsupc___la_LDFLAGS = -static
+#libsupc__convenience_la_SOURCES = $(sources) $(c_sources)
 
 glibcxxinstalldir = $(gxx_include_dir)
 glibcxxinstall_HEADERS = $(headers)
@@ -82,7 +82,6 @@
 # as the occasion call for it.
 AM_CXXFLAGS = \
 	-fno-implicit-templates \
-	$(LIBSUPCXX_PICFLAGS) \
 	$(WARN_CXXFLAGS) \
 	$(OPTIMIZE_CXXFLAGS) \
 	$(CONFIG_CXXFLAGS)
@@ -101,7 +100,7 @@
 
 # LTCOMPILE is copied from LTCXXCOMPILE below.
 LTCOMPILE = $(LIBTOOL) --tag CC --tag disable-shared --mode=compile $(CC) \
-	    $(DEFS) $(C_INCLUDES) $(LIBSUPCXX_PICFLAGS) \
+	    $(DEFS) $(C_INCLUDES)  \
             $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
 
 cp-demangle.c:
