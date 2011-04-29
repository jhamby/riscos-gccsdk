Index: libstdc++-v3/src/Makefile.am
===================================================================
--- libstdc++-v3/src/Makefile.am	(revision 172858)
+++ libstdc++-v3/src/Makefile.am	(working copy)
@@ -27,6 +27,22 @@
 # Cross compiler support.
 toolexeclib_LTLIBRARIES = libstdc++.la
 
+std_HEADERS = \
+	../libsupc++/cxxabi.h ../libsupc++/exception \
+	../libsupc++/initializer_list ../libsupc++/new \
+	../libsupc++/typeinfo 
+
+bits_HEADERS = \
+	../libsupc++/cxxabi_forced.h ../libsupc++/hash_bytes.h \
+	../libsupc++/exception_defines.h ../libsupc++/exception_ptr.h ../libsupc++/nested_exception.h 
+
+headers = $(std_HEADERS) $(bits_HEADERS)
+
+if GLIBCXX_HOSTED
+  c_sources = \
+	cp-demangle.c 
+endif
+
 # Symbol versioning for shared libraries.
 if ENABLE_SYMVERS
 libstdc++-symbols.ver:  ${glibcxx_srcdir}/$(SYMVER_FILE) \
@@ -184,6 +200,57 @@
 inst_sources =
 endif
 
+# Sources present in the libsupc++ directory.
+supc_sources = \
+	array_type_info.cc \
+	atexit_arm.cc \
+	bad_alloc.cc \
+	bad_cast.cc \
+	bad_typeid.cc \
+	class_type_info.cc \
+	del_op.cc \
+	del_opnt.cc \
+	del_opv.cc \
+	del_opvnt.cc \
+	dyncast.cc \
+	eh_alloc.cc \
+	eh_arm.cc \
+	eh_aux_runtime.cc \
+	eh_call.cc \
+	eh_catch.cc \
+	eh_exception.cc \
+	eh_globals.cc \
+	eh_personality.cc \
+	eh_ptr.cc \
+	eh_term_handler.cc \
+	eh_terminate.cc \
+	eh_throw.cc \
+	eh_type.cc \
+	eh_unex_handler.cc \
+	enum_type_info.cc \
+	function_type_info.cc \
+	fundamental_type_info.cc \
+	guard.cc \
+	guard_error.cc \
+	hash_bytes.cc \
+	nested_exception.cc \
+	new_handler.cc \
+	new_op.cc \
+	new_opnt.cc \
+	new_opv.cc \
+	new_opvnt.cc \
+	pbase_type_info.cc \
+	pmem_type_info.cc \
+	pointer_type_info.cc \
+	pure.cc \
+	si_class_type_info.cc \
+	tinfo.cc \
+	tinfo2.cc \
+	vec.cc \
+	vmi_class_type_info.cc \
+	vterminate.cc \
+	$(c_sources)
+
 # Sources present in the src directory, always present.
 sources = \
 	atomic.cc \
@@ -235,20 +302,20 @@
 	future.cc \
 	valarray.cc \
 	${host_sources} \
-	${host_sources_extra}
+	${host_sources_extra} \
+	${supc_sources}
 
+vpath % $(top_srcdir)/libsupc++
 vpath % $(top_srcdir)/src
 vpath % $(top_srcdir)
 
 libstdc___la_SOURCES = $(sources)
 
 libstdc___la_LIBADD = \
-	$(GLIBCXX_LIBS) \
-	$(top_builddir)/libsupc++/libsupc++convenience.la
+	$(GLIBCXX_LIBS)
 
 libstdc___la_DEPENDENCIES = \
-	${version_dep} \
-	$(top_builddir)/libsupc++/libsupc++convenience.la
+	${version_dep}
 
 libstdc___la_LDFLAGS = \
 	-version-info $(libtool_VERSION) ${version_arg} -lm
@@ -409,7 +476,33 @@
 	$(OPTIMIZE_CXXFLAGS) \
 	$(CONFIG_CXXFLAGS)
 
+# Use special rules for pulling things out of libiberty.  These
+# objects should be compiled with the "C" compiler, not the C++
+# compiler, and also should not use the C++ includes.
+C_INCLUDES = -I.. -I$(toplevel_srcdir)/libiberty -I$(toplevel_srcdir)/include
+C_COMPILE = \
+	$(CC) $(DEFS) $(C_INCLUDES) \
+	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
 
+# LTCOMPILE is copied from LTCXXCOMPILE below.
+LTCOMPILE = $(LIBTOOL) --tag CC $(LIBTOOLFLAGS) --mode=compile \
+	    $(CC) $(DEFS) $(C_INCLUDES) $(LIBSUPCXX_PICFLAGS) \
+            $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
+
+cp-demangle.c:
+	rm -f $@
+	$(LN_S) $(toplevel_srcdir)/libiberty/cp-demangle.c $@
+cp-demangle.lo: cp-demangle.c
+	$(LTCOMPILE) -DIN_GLIBCPP_V3 -Wno-error -c $<
+cp-demangle.o: cp-demangle.c
+	$(C_COMPILE) -DIN_GLIBCPP_V3 -Wno-error -c $<
+
+
+nested_exception.lo: nested_exception.cc
+	$(LTCXXCOMPILE) -std=gnu++0x -c $<
+nested_exception.o: nested_exception.cc
+	$(CXXCOMPILE) -std=gnu++0x -c $<
+
 # libstdc++ libtool notes
 
 # 1) Need to explicitly set LTCXXCOMPILE so that AM_CXXFLAGS is
@@ -430,7 +523,7 @@
 # attempt to infer which configuration to use
 LTCXXCOMPILE = $(LIBTOOL) --tag CXX \
 	       $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile \
-	       $(CXX) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
+	       $(CXX) $(INCLUDES) $(TOPLEVEL_INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
 	       $(AM_CXXFLAGS) $(CXXFLAGS)
 
 LTLDFLAGS = $(shell $(SHELL) $(top_srcdir)/../libtool-ldflags $(LDFLAGS))
@@ -481,3 +574,43 @@
 install_debug:
 	(cd ${debugdir} && $(MAKE) \
 	toolexeclibdir=$(glibcxx_toolexeclibdir)/debug install)
+
+# Install notes
+# We have to have rules modified from the default to counteract SUN make
+# prepending each of $(*_HEADERS) with VPATH below.
+stddir = $(gxx_include_dir)
+bitsdir = $(gxx_include_dir)/bits
+
+install-stdHEADERS: $(std_HEADERS)
+	@$(NORMAL_INSTALL)
+	$(mkinstalldirs) $(DESTDIR)$(stddir)
+	@list='$(std_HEADERS)'; for p in $$list; do \
+	  q=`echo $$p | sed -e 's,.*/,,'`; \
+	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
+	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(stddir)/$$q"; \
+	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(stddir)/$$q; \
+	done
+
+install-bitsHEADERS: $(bits_HEADERS)
+	@$(NORMAL_INSTALL)
+	$(mkinstalldirs) $(DESTDIR)$(bitsdir)
+	@list='$(bits_HEADERS)'; for p in $$list; do \
+	  q=`echo $$p | sed -e 's,.*/,,'`; \
+	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
+	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(bitsdir)/$$q"; \
+	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(bitsdir)/$$q; \
+	done
+
+uninstall-stdHEADERS:
+	@$(NORMAL_UNINSTALL)
+	list='$(std_HEADERS)'; for p in $$list; do \
+	  q=`echo $$p | sed -e 's,.*/,,'`; \
+	  rm -f $(DESTDIR)$(stddir)/$$q; \
+	done
+
+uninstall-bitsHEADERS:
+	@$(NORMAL_UNINSTALL)
+	list='$(bits_HEADERS)'; for p in $$list; do \
+	  q=`echo $$p | sed -e 's,.*/,,'`; \
+	  rm -f $(DESTDIR)$(bitsdir)/$$q; \
+	done
