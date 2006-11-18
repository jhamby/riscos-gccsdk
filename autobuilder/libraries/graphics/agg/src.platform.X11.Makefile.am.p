--- src/platform/X11/Makefile.am.old	2005-10-19 19:11:35.000000000 +0100
+++ src/platform/X11/Makefile.am	2006-11-08 23:16:02.000000000 +0000
@@ -3,6 +3,6 @@
 
 libaggplatformX11_la_LDFLAGS = -version-info @AGG_LIB_VERSION@  -L@x_libraries@
 libaggplatformX11_la_SOURCES = agg_platform_support.cpp
-libaggplatformX11_la_CXXFLAGS =  -I$(top_srcdir)/include @x_includes@
+libaggplatformX11_la_CXXFLAGS =  -I$(top_srcdir)/include -I@x_includes@
 libaggplatformX11_la_LIBADD = -lX11
 endif
