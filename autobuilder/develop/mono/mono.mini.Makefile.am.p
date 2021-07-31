--- mono/mini/Makefile.am.orig	2013-07-30 02:51:08.000000000 +0100
+++ mono/mini/Makefile.am	2013-09-13 18:41:42.190728925 +0100
@@ -32,6 +32,27 @@
 	$(GLIB_LIBS) $(LIBICONV)
 endif
 
+if PLATFORM_RISCOS
+mono_sgen_DEPENDENCIES = \
+	$(monodir)/mono/metadata/libmonoruntimesgen-static.la	\
+	$(monodir)/mono/io-layer/libwapi.la	\
+	$(monodir)/mono/utils/libmonoutils.la \
+	$(monodir)/eglib/src/libeglib-static.la
+# We want the above to be the dependencies, but to link with the static equivalents
+# below.
+static_libs=	\
+	$(monodir)/mono/metadata/libmonoruntime-static.la	\
+	$(monodir)/mono/io-layer/.libs/libwapi.a-static	\
+	$(monodir)/mono/utils/.libs/libmonoutils.a-static \
+	$(GLIB_STATIC_LIBS) $(LIBICONV) \
+	$(libgc_static_libs)
+
+sgenstatic_libs = \
+	$(monodir)/mono/metadata/libmonoruntimesgen-static.la	\
+	$(monodir)/mono/io-layer/.libs/libwapi.a-static	\
+	$(monodir)/mono/utils/.libs/libmonoutils.a-static \
+	$(GLIB_STATIC_LIBS) $(LIBICONV)
+else
 static_libs=	\
 	$(monodir)/mono/metadata/libmonoruntime-static.la	\
 	$(monodir)/mono/io-layer/libwapi.la	\
@@ -44,6 +65,7 @@
 	$(monodir)/mono/io-layer/libwapi.la	\
 	$(monodir)/mono/utils/libmonoutils.la \
 	$(GLIB_LIBS) $(LIBICONV)
+endif
 
 CLASS=$(mcs_topdir)/class/lib/net_4_5
 
@@ -63,6 +85,10 @@
 
 AM_CXXFLAGS = $(LLVM_CXXFLAGS) $(GLIB_CFLAGS)
 
+if PLATFORM_RISCOS
+PLATFORM_CFLAGS = -DENABLE_RISCOS_STACKFRAMES
+endif
+
 if HOST_WIN32
 export HOST_CC
 # The mingw math.h has "extern inline" functions that dont appear in libs, so
@@ -94,7 +120,7 @@
 if JIT_SUPPORTED
 
 if SUPPORT_SGEN
-sgen_binaries = mono-sgen
+sgen_binaries = mono-sgen$(EXEEXT)
 sgen_libraries = libmonosgen-2.0.la
 sgen_static_libraries = libmini-static.la $(sgenstatic_libs)
 endif
@@ -102,7 +128,7 @@
 if SUPPORT_BOEHM
 boehm_libraries = libmonoboehm-2.0.la
 boehm_static_libraries = libmini-static.la $(static_libs)
-boehm_binaries  = mono-boehm
+boehm_binaries  = mono-boehm$(EXEEXT)
 endif
 
 #The mono uses sgen, while libmono remains boehm
@@ -111,16 +137,19 @@
 
 if DISABLE_EXECUTABLES
 else
-bin_SCRIPTS = mono
-noinst_SCRIPTS = mono
+bin_SCRIPTS = mono$(EXEEXT)
+noinst_SCRIPTS = mono$(EXEEXT)
 
-mono: mono-$(mono_bin_suffix)
+mono$(EXEEXT): mono-$(mono_bin_suffix)$(EXEEXT)
 	ln -sf $< $@
 
 install-exec-hook:
 	ln -sf $(DESTDIR)$(bindir)/mono-$(mono_bin_suffix) $(DESTDIR)$(bindir)/mono
+if PLATFORM_RISCOS
+else
 	(cd $(DESTDIR)$(libdir); for i in libmono$(libmono_suffix)*; do ln -sf $$i `echo $$i | sed s/$(libmono_suffix)//` ; done)
 endif
+endif
 
 if DISABLE_EXECUTABLES
 else
@@ -250,7 +279,6 @@
 	$(GLIB_LIBS)		\
 	$(LLVMMONOF)		\
 	$(LIBICONV)		\
-	-lm			\
 	$(MONO_DTRACE_OBJECT)
 
 mono_boehm_LDFLAGS = \
@@ -261,7 +289,6 @@
 	$(GLIB_LIBS)		\
 	$(LLVMMONOF)		\
 	$(LIBICONV)		\
-	-lm			\
 	$(MONO_DTRACE_OBJECT)
 
 mono_sgen_LDFLAGS = $(static_flags) -export-dynamic $(monobinldflags) $(monobin_platform_ldflags)
