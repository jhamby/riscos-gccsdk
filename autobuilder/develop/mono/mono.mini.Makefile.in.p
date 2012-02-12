--- mono/mini/Makefile.in.orig	2011-09-15 18:12:42.000000000 +0100
+++ mono/mini/Makefile.in	2012-01-22 15:23:45.000000000 +0000
@@ -867,7 +867,7 @@
 FGREP = @FGREP@
 GETTEXT_MACRO_VERSION = @GETTEXT_MACRO_VERSION@
 GLIB_CFLAGS = @GLIB_CFLAGS@
-GLIB_LIBS = @GLIB_LIBS@
+GLIB_LIBS = -L$(top_builddir)/eglib/src -leglib-static
 GMODULE_CFLAGS = @GMODULE_CFLAGS@
 GMODULE_LIBS = @GMODULE_LIBS@
 GMSGFMT = @GMSGFMT@
@@ -1100,9 +1100,9 @@
 mono_SOURCES = \
 	main.c
 
-mono_CFLAGS = $(AM_CFLAGS) $(BOEHM_DEFINES) $(LIBGC_CFLAGS)
+mono_CFLAGS = $(AM_CFLAGS) $(BOEHM_DEFINES) $(LIBGC_CFLAGS) -DENABLE_RISCOS_STACKFRAMES
 mono_sgen_SOURCES = $(mono_SOURCES)
-mono_sgen_CFLAGS = $(SGEN_DEFINES) $(AM_CFLAGS)
+mono_sgen_CFLAGS = $(SGEN_DEFINES) $(AM_CFLAGS) -DENABLE_RISCOS_STACKFRAMES
 @DTRACE_G_REQUIRED_FALSE@LIBMONO_DTRACE_OBJECT = 
 @DTRACE_G_REQUIRED_TRUE@LIBMONO_DTRACE_OBJECT = .libs/mono-dtrace.$(OBJEXT)
 @DTRACE_G_REQUIRED_FALSE@MONO_DTRACE_OBJECT = 
@@ -1121,7 +1121,6 @@
 	$(MONO_LIB)		\
 	$(GLIB_LIBS)		\
 	$(LLVMMONOF)		\
-	-lm			\
 	$(MONO_DTRACE_OBJECT)
 
 mono_LDFLAGS = \
@@ -1129,9 +1128,9 @@
 
 mono_sgen_LDADD = \
 	$(MONO_SGEN_LIB)	\
-	$(GLIB_LIBS)		\
+@STATIC_MONO_TRUE@	-L$(top_builddir)/eglib/src/.libs -leglib-static	\
+@STATIC_MONO_FALSE@	$(GLIB_LIBS)		\
 	$(LLVMMONOF)		\
-	-lm			\
 	$(MONO_DTRACE_OBJECT)
 
 mono_sgen_LDFLAGS = $(static_flags) -export-dynamic $(monobinldflags) $(monobin_platform_ldflags)
@@ -1459,7 +1458,7 @@
 $(ACLOCAL_M4): @MAINTAINER_MODE_TRUE@ $(am__aclocal_m4_deps)
 	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
 $(am__aclocal_m4_deps):
-install-libLTLIBRARIES: $(lib_LTLIBRARIES)
+install-libLTLIBRARIES: riscos-pic-target
 	@$(NORMAL_INSTALL)
 	test -z "$(libdir)" || $(MKDIR_P) "$(DESTDIR)$(libdir)"
 	@list='$(lib_LTLIBRARIES)'; test -n "$(libdir)" || list=; \
@@ -4614,28 +4613,28 @@
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
 @am__fastdepCC_FALSE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(genmdesc_CFLAGS) $(CFLAGS) -c -o genmdesc-opcodes.obj `if test -f '../metadata/opcodes.c'; then $(CYGPATH_W) '../metadata/opcodes.c'; else $(CYGPATH_W) '$(srcdir)/../metadata/opcodes.c'; fi`
 
-mono-main.o: main.c
+mono-main.o: main.c riscos-static-target riscos-pic-target
 @am__fastdepCC_TRUE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(mono_CFLAGS) $(CFLAGS) -MT mono-main.o -MD -MP -MF $(DEPDIR)/mono-main.Tpo -c -o mono-main.o `test -f 'main.c' || echo '$(srcdir)/'`main.c
 @am__fastdepCC_TRUE@	$(am__mv) $(DEPDIR)/mono-main.Tpo $(DEPDIR)/mono-main.Po
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	source='main.c' object='mono-main.o' libtool=no @AMDEPBACKSLASH@
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
 @am__fastdepCC_FALSE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(mono_CFLAGS) $(CFLAGS) -c -o mono-main.o `test -f 'main.c' || echo '$(srcdir)/'`main.c
 
-mono-main.obj: main.c
+mono-main.obj: main.c riscos-static-target riscos-pic-target
 @am__fastdepCC_TRUE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(mono_CFLAGS) $(CFLAGS) -MT mono-main.obj -MD -MP -MF $(DEPDIR)/mono-main.Tpo -c -o mono-main.obj `if test -f 'main.c'; then $(CYGPATH_W) 'main.c'; else $(CYGPATH_W) '$(srcdir)/main.c'; fi`
 @am__fastdepCC_TRUE@	$(am__mv) $(DEPDIR)/mono-main.Tpo $(DEPDIR)/mono-main.Po
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	source='main.c' object='mono-main.obj' libtool=no @AMDEPBACKSLASH@
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
 @am__fastdepCC_FALSE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(mono_CFLAGS) $(CFLAGS) -c -o mono-main.obj `if test -f 'main.c'; then $(CYGPATH_W) 'main.c'; else $(CYGPATH_W) '$(srcdir)/main.c'; fi`
 
-mono_sgen-main.o: main.c
+mono_sgen-main.o: main.c riscos-static-target riscos-pic-target
 @am__fastdepCC_TRUE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(mono_sgen_CFLAGS) $(CFLAGS) -MT mono_sgen-main.o -MD -MP -MF $(DEPDIR)/mono_sgen-main.Tpo -c -o mono_sgen-main.o `test -f 'main.c' || echo '$(srcdir)/'`main.c
 @am__fastdepCC_TRUE@	$(am__mv) $(DEPDIR)/mono_sgen-main.Tpo $(DEPDIR)/mono_sgen-main.Po
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	source='main.c' object='mono_sgen-main.o' libtool=no @AMDEPBACKSLASH@
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
 @am__fastdepCC_FALSE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(mono_sgen_CFLAGS) $(CFLAGS) -c -o mono_sgen-main.o `test -f 'main.c' || echo '$(srcdir)/'`main.c
 
-mono_sgen-main.obj: main.c
+mono_sgen-main.obj: main.c riscos-static-target riscos-pic-target
 @am__fastdepCC_TRUE@	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(mono_sgen_CFLAGS) $(CFLAGS) -MT mono_sgen-main.obj -MD -MP -MF $(DEPDIR)/mono_sgen-main.Tpo -c -o mono_sgen-main.obj `if test -f 'main.c'; then $(CYGPATH_W) 'main.c'; else $(CYGPATH_W) '$(srcdir)/main.c'; fi`
 @am__fastdepCC_TRUE@	$(am__mv) $(DEPDIR)/mono_sgen-main.Tpo $(DEPDIR)/mono_sgen-main.Po
 @AMDEP_TRUE@@am__fastdepCC_FALSE@	source='main.c' object='mono_sgen-main.obj' libtool=no @AMDEPBACKSLASH@
@@ -5072,6 +5071,37 @@
 tags:
 	etags -o TAGS `find .. -name "*.h" -o -name "*.c"`
 
+.PHONY: riscos-static-target riscos-pic-target
+
+@SUPPORT_SGEN_FALSE@pic_objects =	\
+@SUPPORT_SGEN_FALSE@		$(wildcard $(monodir)/mono/mini/.libs/libmono_2_0_la-*.o) \
+@SUPPORT_SGEN_FALSE@		$(wildcard $(monodir)/eglib/src/.libs/libeglib_la-*.o) \
+@SUPPORT_SGEN_FALSE@		$(wildcard $(monodir)/mono/metadata/.libs/libmonoruntime_la-*.o) \
+@SUPPORT_SGEN_FALSE@		$(wildcard $(monodir)/mono/io-layer/.libs/*.o) \
+@SUPPORT_SGEN_FALSE@		$(wildcard $(monodir)/mono/utils/.libs/*.o)
+
+@SUPPORT_SGEN_TRUE@pic_objects =	\
+@SUPPORT_SGEN_TRUE@		$(wildcard $(monodir)/mono/mini/.libs/libmonosgen_2_0_la-*.o) \
+@SUPPORT_SGEN_TRUE@		$(wildcard $(monodir)/eglib/src/.libs/libeglib_la-*.o) \
+@SUPPORT_SGEN_TRUE@		$(wildcard $(monodir)/mono/metadata/.libs/libmonoruntimesgen_la-*.o) \
+@SUPPORT_SGEN_TRUE@		$(wildcard $(monodir)/mono/io-layer/.libs/*.o) \
+@SUPPORT_SGEN_TRUE@		$(wildcard $(monodir)/mono/utils/.libs/*.o)
+
+riscos-static-target: $(LTLIBRARIES)
+# Replace archive of PIC objects with archive of static objects
+@SUPPORT_SGEN_FALSE@	mv -T ./.libs/libmono-static.a ./.libs/libmono-static.a.orig
+@SUPPORT_SGEN_FALSE@	cp -T ./.libs/libmono-static.a-static ./.libs/libmono-static.a
+@SUPPORT_SGEN_TRUE@	mv -T ./.libs/libmonosgen-static.a ./.libs/libmonosgen-static.a.orig
+@SUPPORT_SGEN_TRUE@	cp -T ./.libs/libmonosgen-static.a-static ./.libs/libmonosgen-static.a
+
+riscos-pic-target: $(lib_LTLIBRARIES)
+@SUPPORT_SGEN_FALSE@	mv -T ./.libs/libmono-2.0.so.0.0.0 ./.libs/libmono-2.0.so.0.0.0.orig
+@SUPPORT_SGEN_FALSE@	$(if $(V),,@echo -e "LD\t$@";) $(CXXLD) $(AM_CXXFLAGS) \
+@SUPPORT_SGEN_FALSE@	$(CXXFLAGS) $(monoldflags) $(LDFLAGS) -shared -o ./.libs/libmono-2.0.so.0.0.0 $(pic_objects) $(LIBS)
+@SUPPORT_SGEN_TRUE@	mv -T ./.libs/libmonosgen-2.0.so.0.0.0 ./.libs/libmonosgen-2.0.so.0.0.0.orig
+@SUPPORT_SGEN_TRUE@	$(if $(V),,@echo -e "LD\t$@";) $(CXXLD) $(AM_CXXFLAGS) \
+@SUPPORT_SGEN_TRUE@	$(CXXFLAGS) $(monoldflags) $(LDFLAGS) -shared -o ./.libs/libmonosgen-2.0.so.0.0.0 $(pic_objects) $(LIBS)
+
 # Tell versions [3.59,3.63) of GNU make to not export all variables.
 # Otherwise a system limit (for SysV at least) may be exceeded.
 .NOEXPORT:
